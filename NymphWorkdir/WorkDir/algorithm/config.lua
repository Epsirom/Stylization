NYMPH_INPUT_MODE = 'package'
NYMPH_SWITCH = {
	simple_param = true,
}
if not NYMPH_INPUT_PACKAGE then 
	NYMPH_INPUT_PACKAGE = 'sea' 
end
if not NYMPH_SEEDS then 
	NYMPH_SEEDS = 1024 
end
if not NYMPH_PATCH_RADIUS then 
	NYMPH_PATCH_RADIUS = 2 
end
NYMPH_DEBUG = true

if not NYMPH_PATCH_ANN_ITERATIONS then 
	NYMPH_PATCH_ANN_ITERATIONS = 5 
end
_patchANN = patchANN
patchANN = function(src, dst, patch_radius, cor)
	return _patchANN(src, dst, patch_radius, cor, NYMPH_PATCH_ANN_ITERATIONS)
end

if NYMPH_DEBUG then
	debug = cprint
else
	debug = function() end
end


if NYMPH_INPUT_MODE == 'package' then
	local pkg = NYMPH_INPUT_PACKAGE
	NYMPH_STYLE = {
		input = 'img/'..pkg..'/style_in.jpg',
		output = 'img/'..pkg..'/style_out.jpg'
	}
	NYMPH_USER = {
		input = 'img/'..pkg..'/nymph_in.jpg'
	}
else
	cprint('[warning]Unknown input mode!')
	os.exit()
end

if NYMPH_SWITCH.simple_param then
	style = NYMPH_STYLE
	nymph = NYMPH_USER
	seeds = NYMPH_SEEDS
	patch_radius = NYMPH_PATCH_RADIUS
	patch_size = patch_radius * 2 + 1
end
