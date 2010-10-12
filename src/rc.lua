-- Multiline header
header = ([=[

	|    _  _ _    | 
	||_|(_|_\(_)|_|| 
     --8<----- v0.42 -----
]=])

color = {
   black = 0,
   red = 1,
   green= 2,
   yellow= 3,
   blue= 4,
   magenta = 5,
   cyan = 6,
   white = 7,
}


-- Status bar
status_bar = Window.new(80, 24, 20, 20)
status_bar:addstr("Blah\n")
status_bar:refresh()


-- MAN unctrl etc...
local status_bar = status_bar

local keys = {
   -- 't' for toggle scrolling
   t = function () status_bar.autoscroll = not status_bar.autoscroll end,
   -- 'h' for hide
   h = function () status_bar.hidden = true end,
   -- 's' for show
   s = function () status_bar.hidden = false end,

   -- <up> for scroll up
   KEY_UP = function () status_bar:scroll(1) end,
   -- <down> for scroll up
   KEY_DOWN = function () status_bar:scroll(-1) end,
   -- on sig winch
      KEY_RESIZE = function ()
		   width, height = get_screen_size()
		   status_bar:addstr("new window size is: " .. width .. "x" .. height .. "\n")
		end,
}

function getch (val, name)

   local f = keys[name]

   if f
   then
      f()
   else
      status_bar:addstr(val .. "\t'" .. name .. "'\n")
      status_bar:refresh()
   end
end
