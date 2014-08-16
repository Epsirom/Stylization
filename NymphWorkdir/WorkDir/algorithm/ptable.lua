-- ptable: orderly point table, ignore repeat points
point = {}
ptable = {}

function point.compare(pt1, pt2)
	local t = pt1[1] - pt2[1]
	if t == 0 then
		t = pt1[2] - pt2[2]
	end
	return t
end

-- return true, index
-- return false, index_to_insert
function ptable.index(ptb, pt)
	local l = 1
	local r = #(ptb)
	local m = l
	local cmp = 0
	while l <= r do
		if point.compare(ptb[l], pt) == 0 then
			return true, l
		end
		if point.compare(ptb[r], pt) == 0 then
			return true, r
		end
		m = math.floor((l + r) / 2)
		cmp = point.compare(ptb[m], pt)
		if cmp < 0 then
			l = m + 1
		elseif cmp > 0 then
			r = m - 1
		else
			return true, m
		end
	end
	return false, l
end

function ptable.insert(ptb, point)
	local found, index = ptable.index(ptb, point)
	if not found then
		table.insert(ptb, index, point)
	end
	return not found, index
end

function ptable.remove(ptb, point)
	local found, index = ptable.index(ptb, point)
	if found then
		return true, table.remove(ptb, index)
	end
	return false, nil
end