local _set_energy = set_energy
local _set_patch_energy = set_patch_energy

set_energy = function(func_name)
	local result = _set_energy(func_name)
	if result ~= 0 then
		if result == 1 then
			cprint('[warning]Energy function not found: '..func_name)
		else
			cprint('[warning]Set energy function failed: unknown error')
		end
	end
end

set_patch_energy = function(func_name)
	local result = _set_patch_energy(func_name)
	if result ~= 0 then
		if result == 1 then
			cprint('[warning]Energy function not found: '..func_name)
		else
			cprint('[warning]Set energy function failed: unknown error')
		end
	end
end

set_energy_pack = function(func_name)
	set_energy(func_name)
	set_patch_energy(func_name)
end

set_rgb = function() set_energy_pack('rgb_naive') end
set_luminance = function() set_energy_pack('luminance_naive') end
set_rgb_factor = function() set_energy_pack('rgb_factor') end
