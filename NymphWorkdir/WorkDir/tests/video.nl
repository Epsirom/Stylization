function Sleep(n)
   os.execute("sleep " .. n)
end

FPS = 25
f = 1 / FPS

showresult()

for i = 0,999 do
	loadimage("style_in", "video/baby/" .. string.sub(i+1000, 2, -1) .. ".jpg")
	loadimage("style_out", "video/baby3/" .. string.sub(i+1000, 2, -1) .. ".jpg")
	loadimage("nymph_in", "video/grass/" .. string.sub(i+1000, 2, -1) .. ".jpg")
	loadimage("nymph_out", "video/guitar/" .. string.sub(i+1000, 2, -1) .. ".jpg")
	syncview()
	Sleep(f)
end
