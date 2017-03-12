local table = table
local math = math
local error = error
local type = type
local select = select
local next = next
local load = load
local tostring = tostring
local setmetatable = setmetatable
local global = _G

local fs = require('be.fs')
local util = require('be.util')

local compile = ...

local M = {
	compile = compile
}
_ENV = M

local function add_dependency (path)
   if not global.dependency then
      return
   end
   if global.root_dir then
      path = fs.ancestor_relative(path, global.root_dir)
   end
   global.dependency(path)
end

do
   local templates = { }
   local template_dirs = { }

   function get_template (template_name)
      if not template_name then
         error('Template must have a name!')
      end
      
      local existing = templates[template_name]
      if existing ~= nil then
         return existing
      end

      local path = fs.find_file(template_name, table.unpack(template_dirs))
      if path then
         add_dependency(path)
         local contents = fs.get_file_contents(path)
         local fn = util.require_load(contents, '@' .. template_name)
         templates[template_name] = fn
         return fn
      end

      path = fs.find_file(template_name .. '.lua', table.unpack(template_dirs))
      if path then
         add_dependency(path)
         local contents = fs.get_file_contents(path)
         local fn = util.require_load(contents, '@' .. template_name .. '.lua')
         templates[template_name] = fn
         return fn
      end

      path = fs.find_file(template_name .. '.blt', table.unpack(template_dirs))
      if path then
         add_dependency(path)
         return register_template_file(path, template_name)
      end

      error('No template found matching \'' .. template_name .. '\'')
   end

   function register_template_dir (path)
      local n = #template_dirs
      for i = 1, n do
         if template_dirs[i] == path then
            return
         end
      end
      template_dirs[n + 1] = fs.canonical(path)
   end

   function register_template_file (path, template_name)
      if not fs.exists(path) then
         error('Template file \'' .. path .. '\' not found!')
      end
      if not template_name then
         template_name = fs.path_stem(path)
         if not template_name then
            error('Template must have a name!')
         end
      end
      if templates[template_name] ~= nil then
         error('A template named \'' .. template_name .. '\' already exists!')
      end
      local content = fs.get_file_contents(path)
      local compiled = compile(content, '@' .. template_name .. '.blt')
      templates[template_name] = compiled
      return compiled
   end

   function register_template_string (string, template_name)
      if not template_name then
         error('Template must have a name!')
      end
      if templates[template_name] ~= nil then
         error('A template named \'' .. template_name .. '\' already exists!')
      end
      local compiled = compile(string, '@' .. template_name .. ' (BLT)')
      templates[template_name] = compiled
      return compiled
   end

   function resolve_template_path (path)
      return fs.resolve_path(path, template_dirs)
         or fs.resolve_path(path .. '.lua', template_dirs)
         or fs.resolve_path(path .. '.blt', template_dirs)
   end
end

local ctx_meta = { }
ctx_meta.__index = ctx_meta

-- function ctx_meta.include (self, template_name)
--    local template = get_template(template_name)
--    if not template then
--       error('Template \'' .. template_name .. '\' not found!')
--    end
--    template(self:get())
-- end

function ctx_meta.push (self, value)
   self.i = self.i + 1
   self.stack[self.i] = value
   self.type = type(value)
   self.deref = self.type == 'table' or self.type == 'userdata'
end

function ctx_meta.pop (self)
   if self.i > 1 then
      self.stack[self.i] = nil
      self.i = self.i - 1
      local value = self.stack[self.i]
      self.type = type(value)
      self.deref = self.type == 'table' or self.type == 'userdata'
   end
end

function ctx_meta.get (self, level)
   if not level then
      level = 0
   end
   if self.i <= level then
      return self.stack[1]
   else
      return self.stack[self.i - level]
   end
end

function ctx_meta.ref (self, key, local_value)
   if local_value ~= nil then
      return local_value
   end

   if not self.deref then
      return nil
   end

   return self.stack[self.i][key]
end

function ctx_meta.write (self, ...)
   for i = 1, select('#', ...) do
      local n = self.n + 1
      local v =  select(i, ...)
      if v ~= nil then
         self.out[n] = tostring(v)
      self.n = n
      end
   end
end

function ctx_meta.coalesce (self)
   local val = table.concat(self.out)
   return val
end

function create_ctx_ (value)
   local self = {
      stack = { },
      i = 0,
      type = 'nil',
      deref = false,
      out = { },
      n = 0
   }
   setmetatable(self, ctx_meta)
   self:push(value)
   return self
end

do
   local cache = { }
   function table_cache_ (value, func)
      local existing = cache[value]
      if existing == nil then
         existing = func()
         cache[value] = existing
      end
      return existing
   end
end

function fif_ (condition, true_value, false_value)
   if condition then
      return true_value
   else
      return false_value
   end
end

function is_sequence (value)
   if type(value) ~= 'table' then
      return false
   end

   local k = next(value)
   local n = 0
   while type(k) == 'number' do
      n = n + 1
      k = next(value, k)
   end
   if k ~= nil then
      return false
   end
   for i = 1, n do
      if value[i] == nil then
         return false
      end
   end
   return true
end

function gsub_escape(pattern)
   return pattern:gsub('[%(%)%.%%%+%-%*%?%[%]%^%$]', '%%%0')
end

function pgsub (str, pattern, ...)
   return str:gsub(gsub_escape(pattern), ...)
end

function explode (str, separator, is_pattern)
   if separator == nil then
      str = str:gsub('\r\n', '\n')
      separator = '[\r\n]'
      is_pattern = true
   end

   local c, s, e = 1, 1
   local results = { }
   local n = 0
   while s do
      s, e = str:find(separator, c, not is_pattern)
      if s then
         n = n + 1
         results[n] = str:sub(c, s - 1)
         c = e + 1
      end
   end

   n = n + 1
   results[n] = str:sub(c)

   return results, n
end

function pad (n, fill)
   if not n or n < 1 then
      return ''
   end
   if not fill then
      fill = ' '
   end
   local fill_size = #fill
   local i = math.floor(n / fill_size)
   local str = fill:rep(i) .. fill:sub(1, n - (fill_size * i))
   return str
end

function rpad (str, n, fill)
   str = str .. pad(n - #str, fill)
   return str
end

function lpad (str, n, fill)
   str = pad(n - #str, fill) .. str
   return str
end

nl = '\n'

return M
