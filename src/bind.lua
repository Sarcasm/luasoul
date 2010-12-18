-- For status bar update in exec()
require("ui")

-- Some keys are not in the TERMINFO database
-- show /etc/inputrc
local TERM = os.getenv("TERM") or ""
-- rxvt-unicode
if string.find(TERM, "rxvt") then
   define_key("Oa",   "C-<up>")
   define_key("Ob",   "C-<down>")
   define_key("Od",   "C-<left>")
   define_key("Oc",   "C-<right>")
   define_key("[b",   "S-<down>")
   define_key("[a",   "S-<up>")
   define_key("[7^",  "C-<home>")
   define_key("[8^",  "C-<end>") -- if not def it's <clearline>
   define_key("[5^",  "C-<PageUp>")
   define_key("[6^",  "C-<PageDown>")

      -- Xterm
else if string.find(TERM, "xterm") then
      define_key("[1;3C", "M-<right>")
      define_key("[1;3D", "M-<left>")
      define_key("[1;3A", "M-<up>")
      define_key("[1;3B", "M-<down>")
   end
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
      luasoul_error"Sorry invalid call to bind(), string and function required.\n"
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

   -- return true if the keybinding is found otherwise return false
   function exec(key, isprintable)
      local sbar = ui.sbar
      local value = k[key]
      local t = type(value)

      if t == "table"
      then
         k = value
         -- append '<key> ' to the Status bar
         ui.sbar:addstr(key .. " ")
         ui.sbar:refresh()
         return true
      end

      if t == "function"
      then
         value()
         -- if the function was found clear the Status bar
         ui.sbar:addch('\n')
         ui.sbar:refresh()
         return true
      end

      if not isprintable
      then
         ui.sbar:addstr("\nNo binding for : " .. key)
         ui.sbar:refresh()
         ui.sbar:addch('\n')    -- next call will refresh the Status bar
      else
         ui.sbar:addch('\n')
         ui.sbar:refresh()
      end

      k = keytable
      return false
   end
end
