syn_counter = {}

require('mod.color')

function get_rgbhsl(img_name, row, col)
	local b, g, r, h, s, l
	b, g, r = mat_3b(img_name, row, col)
	h, s, l = rgbToHsl(r, g, b)
	return r, g, b, h, s, l
end

function synthesize_point(pt)
	local oldpt = {pt[1], pt[2]}
	if pt[1] < patch_radius then
		pt[1] = patch_radius
	end
	if pt[1] >= nymph.input.rows - patch_radius then
		pt[1] = nymph.input.rows - patch_radius - 1
	end
	if pt[2] < patch_radius then
		pt[2] = patch_radius
	end
	if pt[2] >= nymph.input.cols - patch_radius then
		pt[2] = nymph.input.cols - patch_radius - 1
	end
	local srcr, srcg, srcb, srch, srcs, srcl = get_rgbhsl(nymph.input.img, oldpt[1], oldpt[2])
	local cor_row, cor_col = mat_2i(cor, pt[1], pt[2])
	cor_row = cor_row - pt[1] + oldpt[1]
	cor_col = cor_col - pt[2] + oldpt[2]
	local r, g, b, h, s, l = get_rgbhsl(style.output.img, cor_row, cor_col)
	r, g, b = hslToRgb(h * 0.6 + srch * 0.4, s * 0.2 + srcs * 0.8, l * 0.6 + srcl * 0.4)
	set_mat_3b(nymph.output.img, oldpt[1], oldpt[2], b, g, r)
end

function synthesize_patch_by_luminance(center, offset)
	local b, g, r, h, s, l
	local b1, g1, r1, h1, s1, l1
	local srcb, srcg, srcr, srch, srcs, srcl
	local oldb, oldg, oldr, oldh, olds, oldl
	local counter
	for row = center[1] - patch_radius, center[1] + patch_radius do
		for col = center[2] - patch_radius, center[2] + patch_radius do
			srcr, srcg, srcb, srch, srcs, srcl = get_rgbhsl(nymph.input.img, row, col)
			oldr, oldg, oldb, oldh, olds, oldl = get_rgbhsl(nymph.output.img, row, col)
			r, g, b, h, s, l = get_rgbhsl(style.output.img, offset[1] + row, offset[2] + col)
			counter = syn_counter[row + 1][col + 1]
			oldh = (oldh * counter + h) / (counter + 1)
			olds = (olds * counter + s) / (counter + 1)
			oldl = (oldl * counter + l) / (counter + 1)
			syn_counter[row + 1][col + 1] = counter + 1
			r, g, b = hslToRgb(
							oldh * 0.6 + srch * 0.4, 
							olds * 0.2 + srcs * 0.8, 
							oldl * 0.6 + srcl * 0.4)
			set_mat_3b(nymph.output.img, row, col, b, g, r)
		end
	end
end

function synthesize_patch(center, offset)
	local b, g, r
	local srcb, srcg, srcr
	local oldb, oldg, oldr
	local counter
	for row = center[1] - patch_radius, center[1] + patch_radius do
		for col = center[2] - patch_radius, center[2] + patch_radius do
			srcb, srcg, srcr = mat_3b(nymph.input.img, row, col)
			oldb, oldg, oldr = mat_3b(nymph.output.img, row, col)
			b, g, r = mat_3b(style.output.img, offset[1] + row, offset[2] + col)
			counter = syn_counter[row + 1][col + 1]
			oldb = oldb * counter + b
			oldg = oldg * counter + g
			oldr = oldr * counter + r
			syn_counter[row + 1][col + 1] = counter + 1
			counter = counter + 2
			set_mat_3b(nymph.output.img, row, col, (oldb + srcb) / counter, (oldg + srcg) / counter, (oldr + srcr) / counter)
		end
	end
end

function init_synthesizer()
	syn_counter = {}
	for row = 1, nymph.output.rows do
		local tmp = {}
		for col = 1, nymph.output.cols do
			tmp[col] = 0
		end
		syn_counter[row] = tmp
	end
end

function synthesize()
	local centers = nymph.seeds[1].centers
	local offset = nymph.seeds[1].offset
	for i, center in ipairs(centers) do
		synthesize_patch(center, offset)
	end
end