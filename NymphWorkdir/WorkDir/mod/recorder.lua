recorder = {}

function recorder_module_update_default_values(d, p)
	for k, v in pairs(d) do
		if p[k] == nil then
			p[k] = d[k]
		end
	end
end

recorder.create = function(r)
	if type(r)~='table' then
		r = {}
	end
	recorder_module_update_default_values({
		directory = 'temp',
		current_frame = 0
	}, r)
	return r
end

recorder.record = function(r, img)
	local fr = r.current_frame + 1
	r.current_frame = fr
	saveimage('result/'..r.directory..'/'..string.format('%05d', fr)..'.jpg', img)
end