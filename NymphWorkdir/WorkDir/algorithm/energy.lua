local _set_energy = set_energy
local _set_patch_energy = set_patch_energy

set_energy = function(func_name, params)
	local result = _set_energy(func_name, params)
	if result ~= 0 then
		if result == 1 then
			cprint('[warning]Energy function not found: '..func_name)
		else
			cprint('[warning]Set energy function failed: unknown error')
		end
	end
end

set_patch_energy = function(func_name, params)
	local result = _set_patch_energy(func_name, params)
	if result ~= 0 then
		if result == 1 then
			cprint('[warning]Energy function not found: '..func_name)
		else
			cprint('[warning]Set energy function failed: unknown error')
		end
	end
end

set_energy_pack = function(func_name, params)
	set_energy(func_name, params)
	set_patch_energy(func_name, params)
end

set_rgb = function(params) set_energy_pack('rgb', params) end

