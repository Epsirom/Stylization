require('algorithm3.synthesizer')
require('algorithm3.energy')

math.randomseed(os.time())

function init_cor()
	init_synthesizer()
	init_energy()
	nymph.cor = {}
	nymph.energy = {}
	for row = 1, nymph.input.rows do
		local tmp = {}
		for col = 1, nymph.input.cols do
			tmp[col] = {math.random(1, style.input.rows), math.random(1, style.input.cols)}
			do_synthesize(row, col, tmp[col][1], tmp[col][2])
		end
		nymph.cor[row] = tmp
	end
	for row = 1, nymph.input.rows do
		local tmp = {}
		for col = 1, nymph.input.cols do
			local corpt = nymph.cor[row][col]
			tmp[col] = get_energy(row, col, corpt[1], corpt[2])
		end
		nymph.energy[row] = tmp
	end
end

function update_cor(row, col, new_cor)
	local oldpt = nymph.output.img[row][col]
	do_synthesize(row, col, new_cor[1], new_cor[2])
	local teng = get_energy(row, col, new_cor[1], new_cor[2])
	if teng < nymph.energy[row][col] then
		nymph.cor[row][col] = new_cor
		nymph.energy[row][col] = teng
	else
		nymph.output.img[row][col] = oldpt
	end
end

function validate_in_nymph(pt)
	return not ((pt[1] < 1) or (pt[1] > nymph.input.rows) or (pt[2] < 1) or (pt[2] > nymph.input.cols))
end

function validate_in_style(pt)
	return not ((pt[1] < 1) or (pt[1] > style.input.rows) or (pt[2] < 1) or (pt[2] > style.input.cols))
end

function propagation(row, col)
	if row > 1 then
		local cor_up = nymph.cor[row - 1][col]
		local new_cor = {cor_up[1] + 1, cor_up[2]}
		if validate_in_style(new_cor) then
			update_cor(row, col, new_cor)
		end
	end
	if col > 1 then
		local cor_left = nymph.cor[row][col - 1]
		local new_cor = {cor_left[1], cor_left[2] + 1}
		if validate_in_style(new_cor) then
			update_cor(row, col, new_cor)
		end
	end
end

function random_search(row, col, radius, alpha)
	while radius >= 1 do
		local current_radius = math.floor(radius)
		local old_cor = nymph.cor[row][col]
		local row_min, row_max, col_min, col_max
		row_min = math.max(1, old_cor[1] - current_radius)
		row_max = math.min(style.input.rows, old_cor[1] + current_radius)
		col_min = math.max(1, old_cor[2] - current_radius)
		col_max = math.min(style.input.cols, old_cor[2] + current_radius)
		local new_cor = {math.random(row_min, row_max), math.random(col_min, col_max)}
		update_cor(row, col, new_cor)

		radius = radius * alpha
	end
end