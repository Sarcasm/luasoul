require("ui")
require("bind")

local ui = ui
local bind = bind
local exec = exec

buddy_list = {
   "papin_g",
   "chiron_f",
   "senech_c",
}

bind("<right>",     function ()
                       local input = ui.current.input
                       input.index = input.index + 1
                    end)
bind("<left>",     function ()
                       local input = ui.current.input
                       input.index = input.index - 1
                    end)
bind("<up>",        function ()
                       local msg_box = ui.current.msg_box
                       msg_box:scroll(1)
                    end)
bind("<down>",      function ()
                       local msg_box = ui.current.msg_box
                       msg_box:scroll(-1)
                    end)
bind("<home>",      function () ui.current.input.index = 0                end)
bind("<end>",       function ()
                       local input = ui.current.input
                       input.index = #input.buff + 1
                    end)
bind("<PageUp>",    function ()
                       local msg_box = ui.current.msg_box
                       msg_box:scroll(ui.height - 3)
                    end)
bind("<PageDown>",  function ()
                       local msg_box = ui.current.msg_box
                       msg_box:scroll(ui.height + 3)
                    end)
bind("<delete>",    function () ui.current.input:delch()                  end)
bind("<backspace>", function ()
                       local input = ui.current.input

                       if input.index == 1 then return end

                       input.index = input.index - 1
                       input:delch()
                    end)
bind("C-x C-c",     function () quit()                                    end)
bind("C-z",         function () suspend()                                 end)
bind("RET",         function ()
                   -- get current conversation
                   local c = ui.current

                   -- Get buffer content
                   local msg = c.input.buff
                   if #msg <= 1 then return end
                   c.input:erase() -- delete the content of the input

                   -- Change conversation
                   if msg:match("^/")
                   then
                      local newc = ui.get(msg:sub(2)) -- skip '/'
                      ui.set_current(newc)
                      return
                   end

                   -- Display in message box, with the hour
                   ui.disp_msg_me(c.msg_box, msg)
                   c.msg_box:refresh()

                   -- Send message
                   if ns ~= nil and c ~= ui.get("*log*")
                   then
                      ns:send(c.name, msg)
                   end

                   c.input:refresh()
                end)

-- keyboard input is received
function key_received(key, isprintable)
   local input = ui.current.input

   -- Execute key binding
   if not exec(key, isprintable) and isprintable
   then
      -- add input characters
      input:addch(key)
   end
   input:refresh()              -- focus input
end

function beep()
   io.stdout:write('\a')
end

-- START NETSOUL
ns, err_msg = Netsoul{
   socks_pass = os.getenv("SOCKS_PASS"),
   location = "A la maison",
}

if not ns
then
   luasoul_error(err_msg or "Netsoul fail without any message")
else
   local log = ui.get("*log*")
   if ns:connect() == true
   then
      log.msg_box:addstr("Connected !\n")

      for _, bud in ipairs(buddy_list)
      do
         ns:spy_user(bud)
         log.msg_box:addstr("spy " .. bud .. ".\n")
         if log == ui.current
         then
            log.msg_box:refresh()
         end
      end

      if log == ui.current
      then
         log.msg_box:refresh()
         log.input:refresh()    -- restore cursor...
      end
   end
end

function ns:new_msg (login, msg)
   local c = ui.get(login)

   ui.set_title(ui.current.name .. " -=| new message received `" .. login .. "' |=-")
   ui.disp_msg(c.msg_box, login, msg)
   if c == ui.current
   then
      c.msg_box:refresh()
      c.input:refresh()            -- restore cursor...
   end
   beep()
end

function ns:typing_start (login)
   local c = ui.get(login)

   if c == ui.current
   then
      ui.set_title(login .. " is typing...")
      c.input:refresh()         -- restore cursor...
   end
end

function ns:typing_end (login)
   local c = ui.get(login)

   if c == ui.current
   then
      ui.set_title(login)
      c.input:refresh()         -- restore cursor...
   end
end

-- function ns:login (login)
--    status_bar:addstr('\n')
--    status_bar:addstr(login .. " has just log in.")
--    status_bar:refresh()
--    input_field:refresh()        -- restore cursor...
-- end

-- function ns:logout (login)
--    status_bar:addstr('\n')
--    status_bar:addstr(login .. " has just log out.")
--    status_bar:refresh()
--    input_field:refresh()        -- restore cursor...
-- end


-- function ns:status_changed (login, new_status)
--    status_bar:addstr('\n')
--    status_bar:addstr(login .. " is now " .. new_status)
--    status_bar:refresh()
--    input_field:refresh()        -- restore cursor...
-- end

function ns:unknow_event (msg)
   luasoul_error("unknow msg from Netsoul server: `" .. msg .. "'\n")

   local c = ui.get("*log*")
   if c == ui.current
   then
      c.msg_box:refresh()
      c.input:refresh()         -- restore cursor...
   end
end
