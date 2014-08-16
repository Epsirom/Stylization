candidate_image_packages = {
	'farm', 'flower', 'fruit', 'hand', 'sea'
}
using_ld_image = true
using_image_id = 1

NYMPH_INPUT_PACKAGE = candidate_image_packages[using_image_id]
if using_ld_image then
	NYMPH_INPUT_PACKAGE = NYMPH_INPUT_PACKAGE..'_ld'
end

require('algorithm.config')
require('algorithm.utils')

debug('Package: '..NYMPH_INPUT_PACKAGE)

--set_rgb()
--debug('Energy: rgb_naive')

set_luminance()
debug('Energy: luminance')

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

patch_radius = math.floor(math.sqrt(style.input.rows * style.input.cols / 10000) / 2)
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

-- Matrix
mt = {}
mt_remain_list = {}
for i = 1, nymph.input.rows do
	for j = 1, nymph.input.cols do
		local tmp = (i - 1) * nymph.input.cols + j
		mt[tmp] = 0
		if ValidateCenterInNymph({i, j}) then
			table.insert(mt_remain_list, {i,j})
		end
	end
end

function inMT(row, col)
	return mt[(row - 1) * nymph.input.cols + col]
end

function markMT(row, col)
	local i = (row - 1) * nymph.input.cols + col
	if ValidateCenterInNymph({row, col}) then
		ptable.remove(mt_remain_list, {row, col})
	end
	mt[i] = 1
end

function applyCenter(center)
	for row = center[1] - patch_radius, center[1] + patch_radius do
		for col = center[2] - patch_radius, center[2] + patch_radius do
			markMT(row, col)
		end
	end
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
debug('Start PatchANN...')
patchANN('nymph_in', 'style_in', patch_radius, 'cor')
cor = 'cor'
debug('PatchANN done.')

-- Select seeds
debug('Selecting seeds...')
math.randomseed(os.time())


row_max = nymph.input.rows - patch_radius - 1
col_max = nymph.input.cols - patch_radius - 1

mt_remain = #(mt_remain_list)

while mt_remain > 0 do

nymph.seeds = {}
for i = 1, seeds do
	nymph.seeds[i] = {
		centers = {
			{
				math.random(patch_radius, row_max),
				math.random(patch_radius, col_max)
			}
		},
		energy = 0,
		growings = {}
	}
	GrowAround(nymph.seeds[i], nymph.seeds[i].centers[1])
end
debug('Select seeds done.')

-- Seeds filter
debug('Filtering seeds...')
filter_round = 0
remain_seeds = #(nymph.seeds)

function SeedSortComp(a, b)
	return a.energy < b.energy
end

function GrowSeed(seed)
	local growing_count = #(seed.growings)
	local can_grow = false
	local grow_success = false
	if growing_count > 0 then
		can_grow = true
		local point = table.remove(seed.growings)
		table.insert(seed.centers, point)
		local t_energy = energy(nymph.input.img, style.input.img, patch_radius, cor, seed.centers)
		table.remove(seed.centers)
		if t_energy < seed.energy then
			ptable.insert(seed.centers, point)
			seed.energy = t_energy
			GrowAround(seed, point)
			grow_success = true
		end
	end
	return can_grow, grow_success
end

while remain_seeds > 1 do
	filter_round = filter_round + 1
	debug('Round '..filter_round..', remain '..remain_seeds)
	-- Calculate energy
	for i = 1, remain_seeds do
		nymph.seeds[i].energy = energy(nymph.input.img, style.input.img, patch_radius, cor, nymph.seeds[i].centers)
	end
	-- Sort and screen out
	table.sort(nymph.seeds, SeedSortComp)
	seeds_reduce = math.floor(remain_seeds / 2)
	for i = 1, seeds_reduce do
		table.remove(nymph.seeds)
	end
	remain_seeds = remain_seeds - seeds_reduce
	-- Grow seeds
	for i = 1, remain_seeds do
		GrowSeed(nymph.seeds[i])
	end
end

debug('Growing single seed...')
while GrowSeed(nymph.seeds[1]) do 
end
debug('Grow single seed done.')

debug('Filter seeds done.')

debug('Total center points: '..#(nymph.seeds[1].centers))

markpatch(nymph.input.img, nymph.output.img, patch_radius, nymph.seeds[1].centers)
markcorpatch(style.input.img, style.output.img, patch_radius, cor, nymph.seeds[1].centers)
syncview()
showresult()
saveimage('result/most_match_src.jpg', nymph.output.img)
saveimage('result/most_match_cor.jpg', style.output.img)