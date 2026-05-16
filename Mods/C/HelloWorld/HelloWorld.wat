(module
  (type (;0;) (func))
  (type (;1;) (func (param i32 i32)))
  (import "env" "__linear_memory" (memory (;0;) 1))
  (import "clMC" "log" (func $clMC_log (type 1)))
  (import "env" "__indirect_function_table" (table (;0;) 0 funcref))
  (func $Mod_Init (type 0)
    i32.const 1
    i32.const 0
    call $clMC_log
    return)
  (func $Mod_Shutdown (type 0)
    i32.const 1
    i32.const 14
    call $clMC_log
    return)
  (export "Mod_Init" (func $Mod_Init))
  (export "Mod_Shutdown" (func $Mod_Shutdown))
  (data $.L.str (i32.const 0) "Hello World!\0a\00")
  (data $.L.str.1 (i32.const 14) "Goodbye World!\0a\00"))
