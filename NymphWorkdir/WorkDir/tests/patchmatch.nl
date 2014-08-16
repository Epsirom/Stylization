candidate_image_packages = {
	'farm', 'flower', 'fruit', 'hand', 'sea'
}
using_ld_image = true
using_image_id = 3

NYMPH_INPUT_PACKAGE = candidate_image_packages[using_image_id]
if using_ld_image then
	NYMPH_INPUT_PACKAGE = NYMPH_INPUT_PACKAGE..'_ld'
end

NYMPH_PATCH_ANN_ITERATIONS = 5
require('algorithm.config')
require('algorithm.utils')

debug('Package: '..NYMPH_INPUT_PACKAGE)

set_rgb()
debug('Energy: rgb_naive')

--set_luminance()
--debug('Energy: luminance')

--set_rgb_factor()
--debug('Energy: rgb_factor')


function calc_size(obj)
	obj.rows, obj.cols = imgsize(obj.img)
end

-- Load images and prepare nymph_out
debug('Loading images...')

loadimage('style_in', style.input)
loadimage('style_out', style.output)
loadimage('nymph_in', nymph.input)
copymat('nymph_out', 'nymph_in')

style.input = {img = 'style_in', rows = 0, cols = 0}
calc_size(style.input)
style.output = {img = 'style_out', rows = 0, cols = 0}
calc_size(style.output)
nymph.input = {img = 'nymph_in', rows = 0, cols = 0}
calc_size(nymph.input)
nymph.output = {
	img = 'nymph_out', 
	rows = nymph.input.rows, 
	cols = nymph.input.cols
}

debug('Load images done.')

patch_radius = 5 --math.floor(math.sqrt(style.input.rows * style.input.cols / 10000) / 2)
patch_size = patch_radius * 2 + 1

debug('Patch size: '..patch_size)

dirmap = {
	{0, patch_size},
	{0, -patch_size},
	{patch_size, 0},
	{-patch_size, 0}
}

function ValidateCenterInNymph(point)
	return point[1] >= patch_radius and 
		point[1] < nymph.input.rows - patch_radius and 
		point[2] >= patch_radius and 
		point[2] < nymph.input.cols - patch_radius
end

-- point: growing center
function GrowAround(seed, point)
	local cur_point
	for i = 1, 4 do
		cur_point = {point[1] + dirmap[i][1], point[2] + dirmap[i][2]}
		if ValidateCenterInNymph(cur_point) and 
			not ptable.index(seed.centers, cur_point) 
		then
			ptable.insert(seed.growings, cur_point)
		end
	end
end


-- PatchANN
local time_start = os.time()
debug('Start PatchANN...')
patchANN('nymph_in', 'style_in', patch_radius, 'cor')
cor = 'cor'
debug('PatchANN done. Elapsed seconds: '..os.time() - time_start)

show_pm_prob('style_in', cor, patch_radius, 'pm_prob')
show_pm_result('style_in', cor, patch_radius, 'pm_result')
saveimage('result/pm_prob.jpg', 'pm_prob')
saveimage('result/pm_result.jpg', 'pm_result')