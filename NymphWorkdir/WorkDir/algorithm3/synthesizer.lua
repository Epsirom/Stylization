require('mod.color')

function calc_hsl_by_rgb(pt)
	pt[4], pt[5], pt[6] = rgbToHsl(pt[3], pt[2], pt[1])
end

function calc_rgb_by_hsl(pt)
	pt[3], pt[2], pt[1] = hslToRgb(pt[4], pt[5], pt[6])
end

function init_synthesizer()
	do_synthesize = synthesize_mixture
end

function synthesize1(row, col, cor_row, cor_col)
	nymph.output.img[row][col] = style.output.img[cor_row][cor_col]
end

function synthesize_transfer(row, col, cor_row, cor_col)
	local tmp = nymph.output.img[row][col]
	local ni = nymph.input.img[row][col]
	local so = style.output.img[cor_row][cor_col]
	local si = style.input.img[cor_row][cor_col]
	for i = 1, 3 do
		tmp[i] = ni[i] - si[i] + so[i]
	end
	calc_hsl_by_rgb(tmp)
end

function synthesize_means(row, col, cor_row, cor_col)
	local tmp = nymph.output.img[row][col]
	local ni = nymph.input.img[row][col]
	local so = style.output.img[cor_row][cor_col]
	for i = 1, 3 do
		tmp[i] = (ni[i] + so[i] * 3) / 4
	end
	calc_hsl_by_rgb(tmp)
end

function synthesize_mixture(row, col, cor_row, cor_col)
	local tmp = nymph.output.img[row][col]
	local ni = nymph.input.img[row][col]
	local so = style.output.img[cor_row][cor_col]
	local si = style.input.img[cor_row][cor_col]
	tmp[4] = ni[4] * 0.5 + so[4] * 0.5
	tmp[5] = ni[5] * 0.5 + so[5] * 0.5
	tmp[6] = ni[6] * 0.5 + so[6] * 0.5
	calc_rgb_by_hsl(tmp)
end

function synthesize_luminance(row, col, cor_row, cor_col)
	local tmp = nymph.output.img[row][col]
	local ni = nymph.input.img[row][col]
	local so = style.output.img[cor_row][cor_col]
	tmp[6] = (ni[6] + so[6]) / 2
	calc_rgb_by_hsl(tmp)
end