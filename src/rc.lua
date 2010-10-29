-- Multiline header
header = ([=[

	|    _  _ _    | 
	||_|(_|_\(_)|_|| 
     --8<----- v0.42 -----

]=])

header = ([=[

	|    _  _ _    | 
	||_|(_|_\(_)|_||
]=])

local width, height = get_screen_size()

-- Color value
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

-- Set the window title =)
print"]2;luasoul v0.42\a\r"


-- show /etc/inputrc
-- rxvt-unicode
define_key("Oa", "C-<up>")
define_key("Ob", "C-<down>")
define_key("Od", "C-<left>")
define_key("Oc", "C-<right>")
define_key("[C", "<forward-word>")
define_key("[D", "<backward-word>")
-- Xterm
define_key("[1;3C", "<forward-word>")
define_key("[1;3D", "<backward-word>")
define_key("[1;3A", "<up-word>")
define_key("[1;3B", "<down-word>")

-- Create a style for one line bar
bar_style = Style.new{
   -- bold = true,
   -- underline = true,
   -- blink = true,
   -- reverse = true,
   foreground = colors.WHITE,
   background = colors.BLUE,
}

-- Tab bar
tab_bar = Window.new(width, 1, 0, 0)
tab_bar:set_style(bar_style)

-- Status bar
status_bar = Window.new(width, 1, 0, height -2)
status_bar.autoscroll = true
status_bar:set_style(bar_style)

-- Message box
message_box = Window.new(width, height -3, 0, 1)
message_box.autoscroll = true
message_box:set_style{
   bold = false,
   foreground = colors.BLUE,
}
message_box:addstr(header)
message_box:print_colored("     --8<----- v0.42 -----\n\n",
   {
      bold = false,
      foreground = colors.BLACK,
   })
message_box:refresh()

-- put some example login in tab_bar
tab_bar:addstr("login_a | ")
tab_bar:print_colored("login_b", {bold = true, underline = false})
tab_bar:addstr(" | login_c | login_d")
tab_bar:refresh()

-- Input field
input_field = Input.new(width,	    -- width
			1,	    -- height
			0,	    -- begin_x
			height -1,  -- begin_y
			width + 10) -- limit

local keys = {
   -- C-t for toggle scrolling
   ["C-t"] = function () status_bar.autoscroll = not status_bar.autoscroll end,
   -- C-h for hide
   ["C-h"] = function ()
	  status_bar.hidden = true
	  status_bar:refresh()
       end,
   -- C-d for display (show)
   ["C-d"] = function ()
	  status_bar.hidden = false
	  status_bar:refresh()
       end,
   -- C-l for clear window
   ["C-l"] = function () message_box:clear() end,

   -- resize the status bar
   ["C-r"] = function ()
	  status_bar:resize(width, 2)
	  status_bar:move(0, height -3)
	  -- ugly hack for setting the panel on top...
	  status_bar.hidden = false
	  status_bar:refresh()
       end,

   -- Tabulation upcase the input field buffer
   ["TAB"] = function()
		input_field.buff = string.upper(input_field.buff)
	     end,

   -- Enter
   ["RET"] = function ()
		message_box:print_colored(os.date("%k:%M"),
					  {
					     bold = false,
					     underline = true,
					     foreground = colors.BLACK,				     
					  })
		message_box:print_colored(" - me: ",
					 {
					    bold = true,
					    foreground = colors.BLUE,
					 })
		message_box:print_colored(input_field.buff .. "\n",
					 {
					    -- bold = true,
					    foreground = colors.YELLOW,
					 })
	       input_field:driver("CLR_FIELD")
	       message_box:refresh()
	    end,

   ["C-s"] = function()
	       status_bar:addstr("HURRA ! (^S is passed)")
	    end,

   -- For the moment the scrolling is not relevant...
   -- <up> for scroll up
   KEY_UP = function () status_bar:scroll(1) end,
   -- <down> for scroll up
   KEY_DOWN = function () status_bar:scroll(-1) end,

   -- usual keys
   KEY_HOME = function () input_field:driver("BEG_LINE") end,
   KEY_END = function () input_field:driver("END_LINE") end,
   KEY_LEFT = function () input_field:driver("PREV_CHAR") end,
   KEY_RIGHT = function () input_field:driver("NEXT_CHAR") end,
   KEY_DC = function () input_field:driver("DEL_CHAR") end,
   KEY_BACKSPACE = function ()
		      input_field:driver("PREV_CHAR")
		      input_field:driver("DEL_CHAR")
		      -- input_field:driver("DEL_PREV") -- segfault :/
		   end,


   -- on sig winch
   KEY_RESIZE = function ()
		   width, height = get_screen_size()
		   status_bar:addstr("new window size is: " .. width .. "x" .. height .. "\n")
		end,
}

-- This table contain keys sequences and their associated functions
-- local keys = {}

-- Bin a new key
-- Associate a key sequence to a function
function bind(key_sequence, func)
   
end


-- This function is called when a sigwinch is received
function window_resize()
   -- save old dimension
   local old_width, old_height = height, width
   -- get new dimension
   local width, height = get_screen_size()

   tab_bar:resize(width, 1)
   tab_bar:move(0, 0)
   status_bar:resize(width, 1)
   status_bar:move(0, height - 2)
   message_box:resize(width, height -3)
   message_box:move(0, 1)
   input_field:resize(width, 1, width + 10)
   input_field:move(0, height -1)

   -- status_bar:clear()
   -- tab_bar:clear()
   -- message_box:clear()

   message_box:addstr("width = " .. tostring(width) .. "\nheight = " .. tostring(height) .. "\n")



   status_bar:refresh()
   tab_bar:refresh()
   message_box:refresh()
end


function key_received(val, name)
   status_bar:addstr("\n" .. val .. "\t'" .. name .. "'")
   status_bar:refresh()
   local f = keys[name]
   if f
   then
      f()
      status_bar:refresh()
   else
      input_field:driver(val)
   end   
   -- input_field:driver("VALIDATION")
   input_field:refresh()	-- restore the cursor position
end


-- This is the master input (handle keyboard events)
-- the key_received() of this input is called when there is a keyboard event
-- master_input = input_field
