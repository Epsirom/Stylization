require('algorithm.ploader')
require('algorithm.synthesizer2')
require('algorithm.synthesizer')
require('algorithm.mostmatch')
require('mod.progress')
require('mod.recorder')

nymph_loader({
	-- farm=1, flower=2, fruit=3, hand=4, sea=5, 
	-- anime=6, head=7, ship=8, fat=9, cube=10
	package = 5,
	ld = true,
	seeds = 128,
	patch_radius = 2,
	patch_match_iterations = 3,
	debug = true,
	energy = 'rgb',
	rgb_params = {90, 1, 1, 1}
	--rgb_params = {20, 0.0722, 0.7152, 0.2126}
});

patch_radius = math.floor(math.sqrt(nymph.input.rows * nymph.input.cols / 10000) / 2)
patch_size = patch_radius * 2 + 1

debug('Patch size: '..patch_size)


initMT()
init_synthesizer()

min_mt_remain = style.input.cols * style.input.rows / 10
debug('Start segmentation and synthesizing')
max_mt_remain = mt_remain
total_mt_remain = max_mt_remain - min_mt_remain
percentage_step = 1
debug('0%')
percentage = 0
while mt_remain > min_mt_remain do
	--mostmatch(nymph.output.img, style.output.img)
	mostmatch()
	synthesize()
	current_percentage = (max_mt_remain - mt_remain) / total_mt_remain * 100
	while current_percentage > percentage + percentage_step do
		percentage = percentage + percentage_step
		debug(percentage..'%')
	end
end
debug('Segmentation and synthesizing done')

debug('Synthesizing remain unsegmented regions')
for row = 0, nymph.input.rows - 1 do
	for col = 0, nymph.input.cols - 1 do
		if not inMT(row, col) then
			synthesize_point({row, col})
		end
	end
end
debug('All synthesized.')

saveimage('result/synthesis1.jpg', 'nymph_out')
syncview()
showresult()

function GrowAround(seed, point)
	local cur_point
	for i = 1, 4 do
		cur_point = {point[1] + dirmap[i][1], point[2] + dirmap[i][2]}
		if ValidateCenterInNymph(cur_point) and 
			--(not inMT(cur_point[1], cur_point[2])) and
			(not ptable.index(seed.centers, cur_point)) 
		then
			--debug('Grown')
			ptable.insert(seed.growings, cur_point)
		end
	end
end

initMT()
init_synthesizer2()

rc = recorder.create({directory='synthesis/random'})
recorder.record(rc, 'nymph_out')

most_match_round = 1

--[[
for row = patch_radius, nymph.input.rows - patch_radius - 1 do
	for col = patch_radius, nymph.input.cols - patch_radius - 1 do
		debug('Row '..row..', col '..col)
		nymph.seed = {
			centers = {
				{row, col}
			},
			energy = 0,
			growings = {},
			offset = {}
		}
		nymph.seed.offset[1], nymph.seed.offset[2] = get_offset(cor, row, col)
		GrowAround(nymph.seed, nymph.seed.centers[1])
		while GrowSeed(nymph.seed) do 
		end

		for k, v in ipairs(nymph.seed.centers) do
			nymph2_synthesize_patch(v, nymph.seed.offset)
		end

		syncview()
	end
end
]]--


for i = 1, 100 do
	debug('Iteration '.. i)

	local row = math.random(patch_radius, row_max)
	local col = math.random(patch_radius, col_max)
	nymph.seed = {
			centers = {
				{row, col}
			},
			energy = 0,
			growings = {},
			offset = {}
		}
	nymph.seed.offset[1], nymph.seed.offset[2] = get_offset(cor, row, col)
	GrowAround(nymph.seed, nymph.seed.centers[1])
	while GrowSeed(nymph.seed) do 
	end

	for k, v in ipairs(nymph.seed.centers) do
		nymph2_synthesize_patch(v, nymph.seed.offset)
	end

	recorder.record(rc, 'nymph_out')
	syncview()
end
saveimage('result/synthesis2.jpg', 'nymph_out')