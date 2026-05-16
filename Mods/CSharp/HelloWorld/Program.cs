using System;
using System.Runtime.InteropServices;

public class cl_ModClass
{
    [UnmanagedCallersOnly(EntryPoint = "Init")]
    public static int Init(IntPtr api)
    {
        Console.WriteLine("Hello World!");
        return 0;
    }

    [UnmanagedCallersOnly(EntryPoint = "Update")]
    public static void Update(float dt)
    {
        // Console.WriteLine($"Update dt={dt}");
    }

    [UnmanagedCallersOnly(EntryPoint = "Stop")]
    public static void Stop(float dt)
    {
        Console.WriteLine("Goodbye World!");
    }
}