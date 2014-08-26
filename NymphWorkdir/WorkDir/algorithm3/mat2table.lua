require('mod.color')

function get_bgrhsl(img_name, row, col)
	local b, g, r, h, s, l
	b, g, r = mat_3b(img_name, row, col)
	h, s, l = rgbToHsl(r, g, b)
	return {b, g, r, h, s, l}
end

function convert_mat3b_to_table(mat, rows, cols)
	local rtn = {}
	for row = 1, rows do
		local tmp = {}
		for col = 1, cols do
			local pt = get_bgrhsl(mat, row - 1, col - 1)
			tmp[col] = pt
		end
		rtn[row] = tmp
	end
	return rtn
end

function store_table_to_mat(tb, mat, rows, cols)
	for row = 1, rows do
		for col = 1, cols do
			local pt = tb[row][col]
			set_mat_3b(mat, row - 1, col - 1, pt[1], pt[2], pt[3])
		end
	end
end