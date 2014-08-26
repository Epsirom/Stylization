gaussian_raw = {}

function gaussian(row, col)
	return gaussian_raw[math.abs(row) + 1][math.abs(col) + 1]
end

function gaussian2d(s, u, v) -- s: sigma
	local s2 = s^2
	return 1 / (2 * math.pi * s2) * math.exp(-(u^2 + v^2) / 2 / s2)
end

function init_gaussian(sz, s)
	gaussian_raw = {}
	local sum_p = 0
	local factor = 1
	for i = 0, sz do
		local tmp = {}
		for j = 0, sz do
			tmp[j + 1] = gaussian2d(s, i, j)
			if (i == 0) and (j == 0) then
				factor = 1
			elseif (i == 0) or (j == 0) then
				factor = 2
			else
				factor = 4
			end
			sum_p = sum_p + tmp[j + 1] * factor
		end
		gaussian_raw[i + 1] = tmp
	end
	for i = 0, sz do
		for j = 0, sz do
			gaussian_raw[i + 1][j + 1] = gaussian_raw[i + 1][j + 1] / sum_p
		end
	end
end