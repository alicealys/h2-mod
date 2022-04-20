local maps = {
	"af_caves",
	"af_chase",
	"airport",
	"arcadia",
	"boneyard",
	"cliffhanger",
	"contingency",
	"dc_whitehouse",
	"dcburning",
	"dcemp",
	"ending",
	"estate",
	"favela",
	"favela_escape",
	"gulag",
	"invasion",
	"oilrig",
	"roadkill",
	"trainer",
	"museum",
}

for i = 1, #maps do
	local string = "LUA_MENU_SP_LOCATION_" .. maps[i]:upper()
	game:addlocalizedstring(string, string)
end
