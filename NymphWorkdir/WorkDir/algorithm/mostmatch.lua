
math.randomseed(os.time())

function ValidateCenterInNymph(point)
	return point[1] >= patch_radius and 
		point[1] < nymph.input.rows - patch_radius and 
		point[2] >= patch_radius and 
		point[2] < nymph.input.cols - patch_radius
end

-- Matrix

function inMT(row, col)
	return mt[row * nymph.input.cols + col + 1]
	--return mt[(row - 1) * nymph.input.cols + col]
end

function markMT(row, col)
	local i = row * nymph.input.cols + col + 1
	--local i = (row - 1) * nymph.input.cols + col
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

function getPointNotInMT()
	local row = math.random(patch_radius, row_max)
	local col = math.random(patch_radius, col_max)
	while mt[row * nymph.input.cols + col + 1] do
	--while mt[(row - 1) * nymph.input.cols + col] do
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

-- point: growing center
function GrowAround(seed, point)
	local cur_point
	for i = 1, 4 do
		cur_point = {point[1] + dirmap[i][1], point[2] + dirmap[i][2]}
		if ValidateCenterInNymph(cur_point) and 
			(not inMT(cur_point[1], cur_point[2])) and
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

function initMT()
	mt = {}
	mt_remain = 0
	for i = 0, nymph.input.rows - 1 do
		for j = 0, nymph.input.cols - 1 do
			local tmp = i * nymph.input.cols + j + 1
			mt[tmp] = false
			if ValidateCenterInNymph({i, j}) then
				mt_remain = mt_remain + 1
			end
		end
	end
	row_max = nymph.input.rows - patch_radius - 1
	col_max = nymph.input.cols - patch_radius - 1
	dirmap = {
		{0, patch_size},
		{0, -patch_size},
		{patch_size, 0},
		{-patch_size, 0}
	}
end

function generate_seeds()
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
end

function filter_seeds()
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
end

function grow_single_seed()
	while GrowSeed(nymph.seeds[1]) do 
	end

	local tmp_centers = nymph.seeds[1].centers
	local tmp_count = #(tmp_centers)
	for i = 1, tmp_count do
		applyCenter(tmp_centers[i])
	end
end

function mostmatch(patch_mark, corpatch_mark)
	generate_seeds()
	filter_seeds()
	grow_single_seed()
	if patch_mark then
		markpatch(patch_mark, patch_mark, patch_radius, nymph.seeds[1].centers)
	end
	if corpatch_mark then
		markcorpatch(corpatch_mark, corpatch_mark, patch_radius, nymph.seeds[1].offset[1], nymph.seeds[1].offset[2], nymph.seeds[1].centers)
	end
end
