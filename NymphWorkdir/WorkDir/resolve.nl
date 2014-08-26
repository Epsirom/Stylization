require('algorithm.ploader')

nymph_loader({
	package = 5,
	ld = true,
	seeds = 256,
	patch_radius = 2,
	patch_match_iterations = 3,
	debug = true,
	energy = 'rgb',
	rgb_params = {90, 1, 1, 1}
});


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
mt_remain = 0
for i = 1, nymph.input.rows do
	for j = 1, nymph.input.cols do
		local tmp = (i - 1) * nymph.input.cols + j
		mt[tmp] = false
		if ValidateCenterInNymph({i, j}) then
			mt_remain = mt_remain + 1
		end
	end
end

function inMT(row, col)
	return mt[(row - 1) * nymph.input.cols + col]
end

function markMT(row, col)
	local i = (row - 1) * nymph.input.cols + col
	if ValidateCenterInNymph({row, col}) and not mt[i] then
		mt_remain = mt_remain - 1
	end
	mt[i] = true
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
		if (not inMT(cur_point[1], cur_point[2])) and
			ValidateCenterInNymph(cur_point) and 
			(not ptable.index(seed.centers, cur_point)) 
		then
			--debug('Grown')
			ptable.insert(seed.growings, cur_point)
		end
	end
end

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
		local t_energy = energy(nymph.input.img, style.input.img, patch_radius, seed.offset[1], seed.offset[2], seed.centers)
		table.remove(seed.centers)
		--debug('t_energy:'..t_energy..',seed.energy:'..seed.energy)
		if t_energy < seed.energy then
			ptable.insert(seed.centers, point)
			seed.energy = t_energy
			GrowAround(seed, point)
			grow_success = true
		end
	end
	return can_grow, grow_success
end


row_max = nymph.input.rows - patch_radius - 1
col_max = nymph.input.cols - patch_radius - 1

function getPointNotInMT()
	local row = math.random(patch_radius, row_max)
	local col = math.random(patch_radius, col_max)
	while mt[(row - 1) * nymph.input.cols + col] do
		col = col + 1
		if col > col_max then
			col = patch_radius
			row = row + 1
			if row > row_max then
				row = patch_radius
			end
		end
	end
	return {row, col}
end


-- PatchANN
--debug('Start PatchANN...')
--patchANN('nymph_in', 'style_in', patch_radius, 'cor')
--cor = 'cor'
--debug('PatchANN done.')

-- Select seeds
--debug('Selecting seeds...')
math.randomseed(os.time())


copymat(style.output.img, style.input.img)
copymat(nymph.output.img, nymph.input.img)

require('mod.recorder')
rc_nymph = recorder.create({
	directory = 'segment/nymph_in'
})
rc_style = recorder.create({
	directory = 'segment/style_in'
})

function NymphIteration()

	--debug('mt_remain='..mt_remain)

	nymph.seeds = {}
	for i = 1, seeds do
		nymph.seeds[i] = {
			centers = {
				getPointNotInMT()
			},
			energy = 0,
			growings = {},
			offset = {}
		}
		nymph.seeds[i].offset[1], nymph.seeds[i].offset[2] = get_offset(cor, nymph.seeds[i].centers[1][1], nymph.seeds[i].centers[1][2])
		GrowAround(nymph.seeds[i], nymph.seeds[i].centers[1])
	end
	--debug('Select seeds done.')

	-- Seeds filter
	--debug('Filtering seeds...')
	filter_round = 0
	remain_seeds = #(nymph.seeds)


	while remain_seeds > 1 do
		filter_round = filter_round + 1
		--debug('Round '..filter_round..', remain '..remain_seeds)
		-- Calculate energy
		for i = 1, remain_seeds do
			nymph.seeds[i].energy = energy(nymph.input.img, style.input.img, patch_radius, nymph.seeds[i].offset[1], nymph.seeds[i].offset[2], nymph.seeds[i].centers)
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

	--debug('Growing single seed...')
	while GrowSeed(nymph.seeds[1]) do 
	end
	--debug('Grow single seed done.')

	--debug('Filter seeds done.')

	--debug('Total center points: '..#(nymph.seeds[1].centers))

	local tmp_centers = nymph.seeds[1].centers
	local tmp_count = #(tmp_centers)
	for i = 1, tmp_count do
		applyCenter(tmp_centers[i])
	end

	markpatch(nymph.output.img, nymph.output.img, patch_radius, nymph.seeds[1].centers)
	markcorpatch(style.output.img, style.output.img, patch_radius, nymph.seeds[1].offset[1], nymph.seeds[1].offset[2], nymph.seeds[1].centers)
	
	recorder.record(rc_nymph, nymph.output.img)
	recorder.record(rc_style, style.output.img)

	--syncview()
end

min_mt_remain = style.input.cols * style.input.rows / 10
debug('Start segmentation')
max_mt_remain = mt_remain
total_mt_remain = max_mt_remain - min_mt_remain
percentage_step = 1
debug('0%')
percentage = 0
while mt_remain > min_mt_remain do
	NymphIteration()
	current_percentage = (max_mt_remain - mt_remain) / total_mt_remain * 100
	while current_percentage > percentage + percentage_step do
		percentage = percentage + percentage_step
		debug(percentage..'%')
	end
end
debug('Segmentation done')

--saveimage('result/segmentation_src_'..NYMPH_INPUT_PACKAGE..'.jpg', nymph.output.img)
--saveimage('result/segmentation_cor_'..NYMPH_INPUT_PACKAGE..'.jpg', style.output.img)

syncview()
showresult()


--[[
markpatch(nymph.input.img, nymph.output.img, patch_radius, nymph.seeds[1].centers)
markcorpatch(style.input.img, style.output.img, patch_radius, cor, nymph.seeds[1].centers)
syncview()
showresult()
saveimage('result/most_match_src.jpg', nymph.output.img)
saveimage('result/most_match_cor.jpg', style.output.img)
]]--