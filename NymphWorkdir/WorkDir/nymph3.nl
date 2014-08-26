require('mod.progress')
require('mod.recorder')
require('algorithm.ploader')

nymph_loader({
	-- farm=1, flower=2, fruit=3, hand=4, sea=5, anime=6, head=7
	package = 7,
	ld = true,
	seeds = 1024,
	patch_radius = 2,
	patch_match_iterations = 3,
	debug = true,
	energy = 'rgb',
	rgb_params = {100, 1, 1, 1}
});

require('algorithm3.mat2table')
require('algorithm3.synthesizer')
require('algorithm3.energy')
require('algorithm3.matcher')

style.input.img = convert_mat3b_to_table(style.input.img, style.input.rows, style.input.cols)
style.output.img = convert_mat3b_to_table(style.output.img, style.output.rows, style.output.cols)
nymph.input.img = convert_mat3b_to_table(nymph.input.img, nymph.input.rows, nymph.input.cols)
nymph.output.img = convert_mat3b_to_table(nymph.output.img, nymph.output.rows, nymph.output.cols)

rc = recorder.create({directory='synthesis/pixel'})

init_cor()

store_table_to_mat(nymph.output.img, 'nymph_out', nymph.output.rows, nymph.output.cols)
recorder.record(rc, 'nymph_out')
syncview()
showresult()

random_search_radius = math.max(style.input.rows, style.input.cols)

function doit()
	local pg = progress.create({
		min_value = 1,
		max_value = nymph.input.rows,
		step = 1,
		hint_head = '',
		hint_tail = ''
	})
	progress.display(pg)
	for row = 1, nymph.input.rows do
		for col = 1, nymph.input.cols do
			propagation(row, col)
			random_search(row, col, random_search_radius, 0.5)
		end
		if progress.set_value(pg, row) then
			store_table_to_mat(nymph.output.img, 'nymph_out', nymph.output.rows, nymph.output.cols)
			syncview()
			recorder.record(rc, 'nymph_out')
		end
	end

	store_table_to_mat(nymph.output.img, 'nymph_out', nymph.output.rows, nymph.output.cols)
	syncview()
	recorder.record(rc, 'nymph_out')
end

doit()

--nymph3_iterations = 3

--for i = 1, nymph3_iterations do
	
--end

--store_table_to_mat(nymph.output.img, 'nymph_out', nymph.output.rows, nymph.output.cols)

--syncview()