-- Nymph Package Loader

candidate_image_packages = {
	'farm', 'flower', 'fruit', 'hand', 'sea', 
	'anime', 'head', 'ship', 'fat', 'cube'
}

--[[ Use this code fraction to setup loader.

require('algorithm.ploader')

nymph_loader({
	-- farm=1, flower=2, fruit=3, hand=4, sea=5, 
	-- anime=6, head=7, ship=8, fat=9, cube=10
	package = 1,
	ld = true,
	seeds = 1024,
	patch_radius = 2,
	patch_match_iterations = 3,
	debug = true,
	energy = 'rgb',
	rgb_params = {100, 1, 1, 1}
});

]]--

require('algorithm.utils')

function calc_size(obj)
	obj.rows, obj.cols = imgsize(obj.img)
end

function nymph_loader(terms)
	NYMPH_INPUT_PACKAGE = candidate_image_packages[terms['package']]
	if terms['ld'] then
		NYMPH_INPUT_PACKAGE = NYMPH_INPUT_PACKAGE..'_ld'
	end
	
	NYMPH_SEEDS = terms['seeds']

	NYMPH_PATCH_ANN_ITERATIONS = terms['patch_match_iterations']
	NYMPH_PATCH_RADIUS = terms['patch_radius']

	NYMPH_DEBUG = terms['debug']

	require('algorithm.config')

	set_energy_pack(terms['energy'], terms[terms['energy']..'_params'])
	debug('Package: '..NYMPH_INPUT_PACKAGE)
	debug('Energy: '..terms['energy'])

	loadimage('style_in', style.input)
	loadimage('style_out', style.output)
	loadimage('nymph_in', nymph.input)
	copymat('nymph_out', 'nymph_in')
	
	style.input = {img = 'style_in', rows = 0, cols = 0}
	calc_size(style.input)
	style.output = {img = 'style_out', rows = 0, cols = 0}
	calc_size(style.output)
	nymph.input = {img = 'nymph_in', rows = 0, cols = 0}
	calc_size(nymph.input)
	nymph.output = {
		img = 'nymph_out', 
		rows = nymph.input.rows, 
		cols = nymph.input.cols
	}
	init_mat_3b(nymph.output.img, nymph.output.rows, nymph.output.cols, 0)

	cor = 'cor'
	load_cor(NYMPH_INPUT_PACKAGE, cor)

	debug('Images loaded.')
end
