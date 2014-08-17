require('algorithm.ploader')

candidate_image_packages = {
	'farm', 'flower', 'fruit', 'hand', 'sea'
}

nymph_loader({
	package = candidate_image_packages[1],
	ld = true,
	seeds = 1024,
	patch_radius = 2,
	patch_match_iterations = 3,
	debug = true,
	energy = 'rgb',
	rgb_params = {100, 1, 1, 1}
});

patch_radius = math.floor(math.sqrt(style.input.rows * style.input.cols / 10000) / 2)
patch_size = patch_radius * 2 + 1

debug('Patch size: '..patch_size)

dirmap = {
	{0, patch_size},
	{0, -patch_size},
	{patch_size, 0},
	{-patch_size, 0}
}

