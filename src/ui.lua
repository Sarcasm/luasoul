-- Provide some usefull functions for the user interface

-- Color values
local colors = {
   BLACK        = 0,
   RED          = 1,
   GREEN        = 2,
   YELLOW       = 3,
   BLUE         = 4,
   MAGENTA      = 5,
   CYAN         = 6,
   WHITE        = 7,
}

-- Styles
-- Message box style
local mbox_s = Style{
   foreground = colors.BLUE,
}

-- Input field style
local input_s = Style{
   bold = true,
   foreground = colors.BLUE,
}

-- Status bar style
local sbar_s = Style{
   background = colors.BLACK,
   foreground = colors.YELLOW,
}

-- Tab bar style
tbar_s = Style{
   bold = true,
   foreground = colors.WHITE,
   background = colors.BLUE,
}

-- Hour style
hour_s = Style{
   foreground = colors.BLACK,
   underline = true,
}

-- For message content
text_s = Style{
   foreground = colors.YELLOW,
}

-- Error message style
local err_msg_s = Style{
   foreground = colors.BLACK,
   background = colors.YELLOW,
}

-- Table of "conversation", an input with a chatbox
local conversations = {}

ui = {
   -- Current conversation
   current,
   -- Status bar
   sbar,
   -- Tab bar
   tbar,

   -- Global variables
   width,
   height,
   HISTSIZE,

   -- Create or return a conversation
   get = function (name)
            local exist = conversations[name]
            if exist ~= nil then return exist end

            -- Create a new conversation
            local conv = {
               -- Name of conversation
               name = name,

               -- The message box
               msg_box       = Chatbox(ui.width, ui.height - 3, 0, 1, HISTSIZE),

               -- The Input
               input       = Input(ui.width, 1, 0, ui.height - 1),
            }
            -- Set styles
            conv.msg_box.style = mbox_s
            conv.input.style   = input_s

            conversations[name] = conv
            return conv
         end,

   -- Set conversation title
   set_title = function (title, style)
                  if not style
                  then
                     ui.tbar:addstr("\n--( " .. title .. " )--")
                  else
                     ui.tbar:print_colored("\n--( " .. title .. " )--", style)
                  end
                  ui.tbar:refresh()
               end,

   -- Set current conversation
   set_current = function (conv)
                    conv.msg_box:refresh()
                    conv.input:refresh()
                    ui.current = conv

                    ui.set_title(ui.current.name)
                 end,

   -- Print in message box
   -- Example:
   -- 15:04 - <user>: the message is here
   disp_msg = function (msg_box, dest, msg)
                 msg_box:print_colored(os.date("%H:%M"), hour_s)
                 msg_box:addstr(" - " .. dest .. ": ")
                 msg_box:print_colored(msg, text_s)
                 msg_box:addch('\n')
              end,

   disp_msg_me = function (msg_box, msg)
                    msg_box:print_colored(os.date("%H:%M"), hour_s)
                    msg_box:addstr(" - me: ")
                    msg_box:addstr(msg)
                    msg_box:addch('\n')
              end,
}

ui.width, ui.height = get_screen_size()
ui.HISTSIZE = ui.height

-- Status bar
local sbar = Window(ui.width, 1, 0, ui.height -2)
sbar.style = sbar_s
sbar.autoscroll = true
sbar:refresh()
ui.sbar = sbar

-- Tab bar
local tbar = Chatbox(ui.width, 1, 0, 0)
tbar.style = tbar_s
tbar:refresh()
ui.tbar = tbar

-- Create a log console
local log = ui.get("*log*")
ui.set_current(log)

-- Multiline header
local header = ([=[

        |    _  _ _    |
        ||_|(_|_\(_)|_||
]=])

-- print the header
log.msg_box:addstr(header)
log.msg_box:print_colored("     --8<----- v0.42 -----\n\n", {foreground = colors.BLACK})
log.msg_box:refresh()

-- Now if log win exist she become the logging window
function luasoul_error (msg)
   -- log.msg_box:print_colored(msg, err_msg_s)
   ui.current.msg_box:print_colored(msg, err_msg_s)
end

-- Now if log win exist she become the logging window
function luasoul_resize ()
   -- get new dimension
   local width, height = get_screen_size()
   ui.width, ui.height = width, height

   for k, v in pairs(conversations) do
      -- resize message box
      v.msg_box:resize(ui.width, ui.height -3)
      v.msg_box:move(0, 1)

      -- resize input field
      v.input:resize(ui.width, 1)
      v.input:move(0, ui.height - 1)
   end

   -- Resize Status bar
   sbar:resize(ui.width, 1)
   sbar:move(0, ui.height -2)
   sbar:refresh()

   -- Resize Tab bar
   tbar:resize(ui.width, 1)
   tbar:move(0, 0)
   tbar:refresh()

   -- Refresh only the current conversation
   ui.current.msg_box:refresh()
   ui.current.input:refresh()   -- move cursor in current input
end


-- Move cursor in log input
log.input:refresh()
