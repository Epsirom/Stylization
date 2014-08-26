require('mod.color')

function get_rgbhsl(img_name, row, col)
	local b, g, r, h, s, l
	b, g, r = mat_3b(img_name, row, col)
	h, s, l = rgbToHsl(r, g, b)
	return r, g, b, h, s, l
end

function init_synthesizer2()
	nymph.output.energy = {}
	for row = 1, nymph.input.rows do
		local tmp = {}
		for col = 1, nymph.input.cols do
			tmp[col] = 0
		end
		nymph.output.energy[row] = tmp
	end
end

function c3_distance(pt1, pt2, fac)
	if fac == nil then
		fac = {1, 1, 1}
	end
	local rtn = 0
	for i = 1, 3 do
		rtn = rtn + (math.abs(pt1[i] - pt2[i]) * fac[i])
	end
	return rtn
end

function pt9_distance(pts1, pts2, fac)
	if fac == nil then
		fac = {1, 1, 1}
	end
	local center_diff = {}
	for i = 1, 3 do
		center_diff[i] = pts1[2][2][i] - pts2[2][2][i]
	end
	local rtn = 0
	for i = 1, 3 do
		for j = 1, 3 do
			for k = 1, 3 do
				rtn = rtn + (math.abs(pts1[i][j][k] - pts2[i][j][k] - center_diff[k]) * fac[k])
			end
		end
	end
	return rtn
end

function get_c3(img, row, col)
	local tmp = {}
	tmp[1], tmp[2], tmp[3] = mat_3b(img, row, col)
	return tmp
end

function get_9pt(img, row, col)
	local rtn = {}
	for i = -1, 1 do
		local tmp = {}
		for j = -1, 1 do
			local trow, tcol = row + i, col + j
			local ttmp = {0, 0, 0}
			if (trow >= 0) and (trow < nymph.input.rows) and 
				(tcol >= 0) and (tcol < nymph.input.cols) then
				ttmp[1], ttmp[2], ttmp[3] = mat_3b(img, trow, tcol)
			end
			tmp[j + 2] = ttmp
		end
		rtn[i + 2] = tmp
	end
	return rtn
end

function get_c6(img, row, col)
	local tmp = {}
	tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6] = get_rgbhsl(img, row, col)
	return tmp
end

function get_hsl_by_bgr(pt)
	local rtn = {}
	rtn[1], rtn[2], rtn[3] = rgbToHsl(pt[3], pt[2], pt[1])
	return rtn
end

function get_energy(row, col, crow, ccol)
	local so_pt = get_c3(style.output.img, crow, ccol)
	so_pt = get_hsl_by_bgr(so_pt)

	local ni_9pt = get_9pt(nymph.input.img, row, col)
	local no_9pt = get_9pt(nymph.output.img, row, col)

	for i = 1, 3 do
		for j = 1, 3 do
			ni_9pt[i][j] = get_hsl_by_bgr(ni_9pt[i][j])
			no_9pt[i][j] = get_hsl_by_bgr(no_9pt[i][j])
		end
	end

	local so_distance = c3_distance(so_pt, no_9pt[2][2], {0, 0, 1})
	local so_factor = 1
	local ni_distance = c3_distance(ni_9pt[2][2], no_9pt[2][2], {0, 0, 1})
	local ni_factor = 1
	local pt_distance = pt9_distance(ni_9pt, no_9pt, {0, 0, 1})
	local pt_factor = 1

	return so_distance * so_factor + ni_distance * ni_factor + pt_distance * pt_factor
end

function nymph2_synthesize_point_(row, col, crow, ccol)
	local so_pt = get_c6(style.output.img, crow, ccol)
	local ni_pt = get_c6(nymph.input.img, row, col)
	local no_pt = get_c6(nymph.output.img, row, col)
	
	if not inMT(row, col) then
		set_mat_3b(nymph.output.img, row, col, 
				(so_pt[3] + ni_pt[3]) / 2,
				(so_pt[2] + ni_pt[2]) / 2,
				(so_pt[1] + ni_pt[1]) / 2)
	else
		set_mat_3b(nymph.output.img, row, col, 
				(no_pt[3] * 2 + so_pt[3]) / 3,
				(no_pt[2] * 2 + so_pt[2]) / 3,
				(no_pt[1] * 2 + so_pt[1]) / 3)
	end
end

function nymph2_synthesize_point(row, col, crow, ccol)
	local oldpt = {}
	oldpt[1], oldpt[2], oldpt[3] = mat_3b(nymph.output.img, row, col)
	nymph2_synthesize_point_(row, col, crow, ccol)
	local tenergy = get_energy(row, col, crow, ccol)
	if not inMT(row, col) then
		markMT(row, col)
		nymph.output.energy[row + 1][col + 1] = tenergy
	else
		if tenergy > nymph.output.energy[row + 1][col + 1] then
			set_mat_3b(nymph.output.img, row, col, oldpt[1], oldpt[2], oldpt[3])
		else
			nymph.output.energy[row + 1][col + 1] = tenergy
		end
	end
end

function nymph2_synthesize_patch(center, offset)
	local crow, ccol
	for row = center[1] - patch_radius, center[1] + patch_radius do
		for col = center[2] - patch_radius, center[2] + patch_radius do
			crow = row + offset[1]
			ccol = col + offset[2]
			if (col >= 0) and (col < nymph.input.cols) and 
				(row >= 0) and (row < nymph.input.rows) and 
				(crow >= 0) and (crow < nymph.input.rows) and
				(ccol >= 0) and (ccol < nymph.input.cols) then
				nymph2_synthesize_point(row, col, crow, ccol)
			end
		end
	end
end