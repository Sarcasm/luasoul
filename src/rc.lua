-- Multiline header
header = ([=[

	|    _  _ _    | 
	||_|(_|_\(_)|_||
]=])

-- http://www.reddit.com/r/reddit.com/comments/87gf7/notice_how_theyre_starting_to_crack_down_on_the/c08h5lu

header = ([=[

          /◝o     
       (¤_¤)
       .[__].
█▄▄ ███ █▄▄ █▄█▄█ █▄█ ▀█▀

☉_☉

‹•.•›

ō_ō`

╔═╗
║▓▒░░░░░░░░░░░░░░░░░░
╚═╝
IMMA CHARGIN MAH LAZER!

I ♥ you

 .'"'.        ___,,,___        .'``.
: (\  `."'"```         ```"'"-'  /) ;
 :  \                         `./  .'
  `.                            :.'
    /       __)        __)      \

   |        (●)       (●)        |
   |         /         \         |
   |       /             \       |
    \     |      .-.      |     /
     `.   | . . /   \ . . |   .'
       `-._\.'.(     ).'./_.-'
           `\'  `._.'  '/'
             `. --'-- .'
               `-...-'


               ("\ '' /").___..--' ' "`-._  
               `●_ ●  )   `-.  (       ).`-.__.`) 
               (_Y_.)'    ._   )  `._ `. ``-..-' 
             _..`--'_. .-_/  /--'_.'  .'          
            (i l).-' '    ((i). '   ((!.-'


    ____
   (►..◄)
    ||||

* PUNISHER *

]=])

function luasoul_error (msg)
   io.stderr:write(msg)
end

buddy_list = {
   "login_1",
   "login_2",
   "login_3",
   "login_4",
   "login_5",
   "login_6",
   "login_7",
}

local width, height = get_screen_size()
local HISTSIZE = height -- one page history

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

-- Set the window title
-- print"]2;luasoul v0.42\a\r"
-- clear()

-- show /etc/inputrc
local TERM = os.getenv("TERM") or ""
-- rxvt-unicode
if string.find(TERM, "rxvt") then
   define_key("Oa",	"C-<up>")
   define_key("Ob",	"C-<down>")
   define_key("Od",	"C-<left>")
   define_key("Oc",	"C-<right>")
   define_key("[b",	"S-<down>")
   define_key("[a",	"S-<up>")
   -- define_key("[C",	"M-<right>")
   -- define_key("[D", "M-<left>")
   -- define_key("[C",	"M-<up>")
   -- define_key("[D", "M-<down>")
   define_key("[7^", "C-<home>")
   define_key("[8^", "C-<end>") -- if not def it's <clearline>
   define_key("[5^", "C-<PageUp>")
   define_key("[6^", "C-<PageDown>")

      -- Xterm
else if string.find(TERM, "xterm") then
      define_key("[1;3C", "M-<right>")
      define_key("[1;3D", "M-<left>")
      define_key("[1;3A", "M-<up>")
      define_key("[1;3B", "M-<down>")
   end
end

-- Create styles
hour_style = Style.new{
   bold = false,
   underline = true,
   foreground = colors.BLACK,
}
me_style = Style.new{
   bold = false,
}
text_style = Style.new{
   foreground = colors.YELLOW,
}
error_style = Style.new{
   foreground = colors.RED,
   background = colors.YELLOW,
}

-- Tab styles
tab_style = Style.new{
   bold = true,
   foreground = colors.WHITE,
   background = colors.BLUE,
}
tab_focus_style = Style.new{
   foreground = colors.WHITE,
   background = colors.BLACK,
}

-- Tab bar
tab_bar = Chatbox.new(width, 1, 0, 0, #buddy_list - 1)
tab_bar.style = tab_style

-- the buddy number `n' is the buddy to focus
function write_line (n)
   local b = buddy_list
   for i, v in ipairs(buddy_list)
   do
      tab_bar:addstr(" | ")
      if i == n
      then
	 tab_bar:print_colored(b[i], tab_focus_style)
      else
      	 tab_bar:addstr(b[i])
      end
   end
   if n == #buddy_list then
      tab_bar:addstr(" |")
   else
      tab_bar:addstr(" |\n")
   end
end

function init_tab_bar ()
   local i = 1
   local len = #buddy_list
   while i <= len
   do
      write_line(i)
      i = i + 1
   end
end
init_tab_bar()
tab_bar:scroll(#buddy_list)	-- focus first in list
tab_bar:refresh()

-- Message box
-- Chatbox.new(width, height, begin_x, begin_y[, history_size])
message_box = Chatbox.new(width, height - 3, 0, 1, HISTSIZE)
message_box.style = {bold = false, foreground = colors.BLUE}
-- print the header
message_box:addstr(header)
message_box:print_colored("     --8<----- v0.42 -----\n\n", {bold = false, foreground = colors.BLACK})
message_box:refresh()


-- if ncurses is ok print to message box
function luasoul_error (msg)
   message_box:print_colored(msg, error_style)
   message_box:addch('\n')
end


-- Status bar
status_bar = Window.new(width, 1, 0, height -2)
status_bar.style = {foreground = colors.YELLOW, background = colors.BLACK}
status_bar.autoscroll = true
status_bar:refresh()

-- Input field
input_field = Input.new(width, 1, 0, height -1, 100)
input_field.style = {foreground = colors.BLUE, bold = true}
input_field:refresh()

kill_ring = ""

-- This function is called when a sigwinch is received
function window_resize()
   -- save old dimension
   local old_width, old_height = height, width
   -- get new dimension
   local width, height = get_screen_size()

   message_box:resize(width, height - 3)
   message_box:move(0, 1)	-- useless ?
   message_box:addstr("width = " .. tostring(width) .. "\nheight = " .. tostring(height) .. "\n")

   status_bar:resize(width, 1)
   status_bar:move(0, height -2)

   input_field:resize(width, 1)
   input_field:move(0, height -1)

   status_bar:refresh()
   message_box:refresh()
   input_field:refresh()
end

-- This table contain keys sequences and their associated functions
local keytable = {}

-- Bind a new key
-- Associate a key sequence to a function
function bind(key_sequence, func)
   -- little check
   if type(key_sequence) ~= "string" and type(key_sequence) ~= "number"
      or type(func) ~= "function"
   then
      message_box:addstr"Sorry invalid call to bind(), string and function required.\n"
      message_box:refresh()
      return
   end

   local k = keytable
   local lastk
   local lastw
   -- For each word separated by a space create a new table key
   for w in string.gmatch(key_sequence, "[^ ]+") do
      -- for space binding
      if w == "SPC" then w = " " end

      k[w] = type(k[w]) == "table" and k[w] or {}
      lastk = k
      lastw = w
      k = k[w]
   end
   lastk[lastw] = func
end

do
   local k = keytable

   -- return true if the keybinding is found, else false
   function exec(key)
      local t = type(k[key])

      if t == "function"
      then
	 status_bar:addstr(key .. " ")
	 status_bar:refresh()
	 -- ugly clear...
	 status_bar:addstr("\n")
	 k[key]()
	 return true
      end

      if t == "table"
      then
	 status_bar:addstr(key .. " ")
	 status_bar:refresh()
	 k = k[key]
	 return true
      end
      status_bar:refresh()
      k = keytable
      return false
   end
end
   
-- START KEY BINDING
-- cursor motion & co
bind("<right>",		function () input_field:move_cursor(1)			end)
bind("<left>",		function () input_field.index = input_field.index - 1	end)
bind("<up>",		function () message_box:scroll(1)			end)
bind("<down>",		function () message_box:scroll(-1)			end)
bind("<home>",		function () input_field:move_cursor(-1000)		end)
bind("<end>",		function () input_field.index = #input_field.buff + 1	end)
bind("<PageUp>",	function () message_box:scroll(height)			end)
bind("<PageDown>",	function () message_box:scroll(-height)			end)
bind("<delete>",	function () input_field:remove(1)			end)
bind("<backspace>",	function () input_field:remove(-1)			end)
bind("C-y",		function () input_field:addstr(kill_ring)		end)

bind("C-x C-z",		function () suspend()					end)

bind("M-<right>",	function ()
			   local i = input_field.index
			   local buff = input_field.buff
			   -- find next space
			   local f = string.find(buff, "%S%s", i)
			   if f then input_field.index = f + 1
			   else
			      input_field.index = #buff + 1
			   end
			end)

-- not really beautiful...
bind("M-<left>",	function ()
			   local i = input_field.index -1
			   local buff = input_field.buff

			   local last_match = 1
			   local f = 1
			   -- find next space
			   while true do
			      f = (string.find(buff, "%s%S", f) or i) + 1
			      if f > i
			      then
				 input_field.index = last_match
				 return
			      end
			      last_match = f
			   end
			end)

bind("C-<left>",	function () tab_bar:scroll(1)			end)
bind("C-<right>",	function () tab_bar:scroll(-1)			end)

-- Enter
bind("RET",		function ()
			   message_box:print_colored(os.date("%H:%M"), hour_style)
			   message_box:print_colored(" - me: ", me_style)
			   local msg = input_field.buff
			   -- Have fun !
			   -- http://en.wikipedia.org/wiki/Interrobang
			   -- http://superuser.com/questions/52671/how-do-i-create-unicode-smilies-like-
			   msg = msg:gsub("?!", "‽")
			   msg = msg:gsub(":%)", "☺")
			   message_box:print_colored(msg .. "\n", text_style)
			   input_field:erase() -- delete the content of the buffer
			   message_box:refresh()
			end)
bind("C-k",		function ()
			   local tmp = input_field:remove(#input_field.buff) or ""
			   if #tmp > 0 then kill_ring = tmp end
			end)
bind("C-l",		function ()
			   clear()
			   status_bar:refresh()
			   message_box:refresh()
			end)
-- END KEY BINDING

-- keyboard input is received
function key_received(val, name, printable)
   if not exec(name)
   then
      if printable
      then
   	 input_field:addch(name)
      else
	 message_box:addstr(name .. '\n')
	 message_box:refresh()
      end
   end   
   input_field:refresh()
end
