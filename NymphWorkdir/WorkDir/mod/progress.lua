progress = {}

function progress_module_update_default_values(d, p)
	for k, v in pairs(d) do
		if p[k] == nil then
			p[k] = d[k]
		end
	end
end

progress.create = function(p)
	if type(p)~='table' then
		p = {}
	end
	progress_module_update_default_values({
		min_value = 0,
		max_value = 100,
		step = 1,
		hint_head = '',
		hint_tail = ''
	}, p)
	p.total_value = p.max_value - p.min_value
	p.current_value = p.min_value
	p.percentage = 0
	return p
end

progress.display = function(p, printfunc)
	if not printfunc then
		printfunc = cprint
	end
	printfunc(p.hint_head..' '..p.percentage..'% '..p.hint_tail)
end

progress.add = function(p, v, printfunc)
	p.current_value = p.current_value + v
	local tmp = (p.current_value - p.min_value) / p.total_value * 100
	while tmp > p.percentage + p.step do
		p.percentage = p.percentage + p.step
		progress.display(p, printfunc)
	end
end

progress.set_value = function(p, v, printfunc)
	p.current_value = v
	local tmp = (p.current_value - p.min_value) / p.total_value * 100
	while tmp > p.percentage + p.step do
		p.percentage = p.percentage + p.step
		progress.display(p, printfunc)
	end
end