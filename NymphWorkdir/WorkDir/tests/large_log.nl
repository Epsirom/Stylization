function Sleep(n)
   os.execute("sleep " .. n)
end

for i = 1, 100000 do
	cprint('Test '..i)
	-- Sleep(0.0001)
end