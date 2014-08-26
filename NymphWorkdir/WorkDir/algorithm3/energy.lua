function init_energy()
	get_energy = energy_mix
end

function energy_mix(row, col, cor_row, cor_col)
	return (3 * energy_nymph_input_rgb(row, col, cor_row, cor_col)) +
			--(5 * energy_nymph_input_luminance(row, col, cor_row, cor_col)) +
			(5 * energy_style_output_luminance(row, col, cor_row, cor_col)) +
			(3 * energy_input_spatial_coherence_rgb(row, col, cor_row, cor_col)) +
			0--(0.5 * energy_adjacent(row, col, cor_row, cor_col))
end

function energy_adjacent(row, col, cor_row, cor_col)
	local dir_map = {
		{1, 0},
		{-1, 0},
		{0, 1},
		{0, -1}
	}
	local rtn = 0
	for i = 1, 4 do
		local d = dir_map[i]
		local trow, tcol = row + d[1], col + d[2]
		if (trow >= 1) and (trow <= nymph.input.rows) and (tcol >= 1) and (tcol <= nymph.input.cols) then
			local corpt = nymph.cor[trow][tcol]
			rtn = rtn + math.abs(cor_row - corpt[1] + d[1]) + math.abs(cor_col - corpt[2] + d[2])
		end
	end
	return rtn
end

function energy_nymph_input_rgb(row, col, cor_row, cor_col)
	local pt = nymph.output.img[row][col]
	local inpt = nymph.input.img[row][col]
	local rtn = 0
	for i = 1, 3 do
		rtn = rtn + math.abs(pt[i] - inpt[i])
	end
	return rtn
end

function energy_nymph_input_hsl(row, col, cor_row, cor_col)
	local pt = nymph.output.img[row][col]
	local inpt = nymph.input.img[row][col]
	local rtn = 0
	for i = 4, 6 do
		rtn = rtn + math.abs(pt[i] - inpt[i])
	end
	return rtn
end

function energy_nymph_input_luminance(row, col, cor_row, cor_col)
	local pt = nymph.output.img[row][col]
	local inpt = nymph.input.img[row][col]
	return math.abs(pt[6] - inpt[6])
end

function energy_style_output_rgb(row, col, cor_row, cor_col)
	local pt = nymph.output.img[row][col]
	local pt2 = style.output.img[cor_row][cor_col]
	local rtn = 0
	for i = 1, 3 do
		rtn = rtn + math.abs(pt[i] - pt2[i])
	end
	return rtn
end

function energy_style_output_hsl(row, col, cor_row, cor_col)
	local pt = nymph.output.img[row][col]
	local pt2 = style.output.img[cor_row][cor_col]
	local rtn = 0
	for i = 4, 6 do
		rtn = rtn + math.abs(pt[i] - pt2[i])
	end
	return rtn
end

function energy_style_output_luminance(row, col, cor_row, cor_col)
	local pt = nymph.output.img[row][col]
	local pt2 = style.output.img[cor_row][cor_col]
	return math.abs(pt[6] - pt2[6])
end

function get_patch_by_center(pk, crow, ccol, r)
	local rtn = {}
	for row = crow - r, crow + r do
		local tmp = {}
		local trow = row + r - crow + 1
		for col = ccol - r, ccol + r do
			local tcol = col + r - ccol + 1
			if (row >= 1) and (row <= pk.rows) and (col >= 1) and (col <= pk.cols) then
				tmp[tcol] = pk.img[row][col]
			else
				tmp[tcol] = {-1}
			end
		end
		rtn[trow] = tmp
	end
	return rtn
end

function energy_input_spatial_coherence_rgb(row, col, cor_row, cor_col)
	local patch_radius = 3
	local pc_nymph = get_patch_by_center(nymph.input, row, col, patch_radius)
	local pc_style = get_patch_by_center(style.input, cor_row, cor_col, patch_radius)
	local center_dist = {}
	local ct = patch_radius + 1
	local nymph_center = pc_nymph[ct][ct]
	local style_center = pc_style[ct][ct]
	for i = 1, 3 do
		center_dist[i] = nymph_center[i] - style_center[i]
	end
	local sz = patch_radius * 2 + 1
	local counter = 0
	local rtn = 0
	for row = 1, sz do
		for col = 1, sz do
			local npt = pc_nymph[row][col]
			local spt = pc_style[row][col]
			if (npt[1] ~= -1) and (spt[1] ~= -1) then
				counter = counter + 1
				for i = 1, 3 do
					rtn = rtn + math.abs(npt[i] - spt[i] - center_dist[i])
				end
			end
		end
	end
	return rtn / counter
end

function energy_input_spatial_coherence_luminance(row, col, cor_row, cor_col)
	local patch_radius = 3
	local pc_nymph = get_patch_by_center(nymph.input, row, col, patch_radius)
	local pc_style = get_patch_by_center(style.input, cor_row, cor_col, patch_radius)
	local center_dist = {}
	local ct = patch_radius + 1
	local nymph_center = pc_nymph[ct][ct]
	local style_center = pc_style[ct][ct]
	for i = 1, 6 do
		center_dist[i] = nymph_center[i] - style_center[i]
	end
	local sz = patch_radius * 2 + 1
	local counter = 0
	local rtn = 0
	for row = 1, sz do
		for col = 1, sz do
			local npt = pc_nymph[row][col]
			local spt = pc_style[row][col]
			if (npt[1] ~= -1) and (spt[1] ~= -1) then
				counter = counter + 1
				for i = 6, 6 do
					rtn = rtn + math.abs(npt[i] - spt[i] - center_dist[i])
				end
			end
		end
	end
	return rtn / counter
end