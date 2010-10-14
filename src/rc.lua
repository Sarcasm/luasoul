-- Multiline header
header = ([=[

	|    _  _ _    | 
	||_|(_|_\(_)|_|| 
     --8<----- v0.42 -----
]=])

local width, height = get_screen_size()

local colors = {
   BLACK	= 0,
   RED		= 1,
   GREEN	= 2,
   YELLOW	= 3,
   BLUE		= 4,
   MAGENTA	= 5,
   CYAN		= 6,
   WHITE	= 7,
}

-- tab bar
tab_bar = Window.new(width, 1, 0, 0)
tab_bar:set_style{
   bold = true,
   underline = true,
   blink = true,
   reverse = true,
   foreground = colors.WHITE,
   background = colors.BLUE,
}
tab_bar:addstr("login_a | login_b | login_c | login_d")
tab_bar:refresh()

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
   -- 'L' for clear
   L = function () status_bar:clear() end,

   -- For the moment the scrolling is not relevant...
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
