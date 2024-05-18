import frida
import sys

def on_message(message, data):
    print(message)

# JavaScript code to be injected
jscode = """

var file = new File("dump.txt", "a")

//hook GdipSetSmoothingMode, add counter and print out counter each when it get called

var counter = 0;
const base = Module.findBaseAddress('CaptainHook.exe')

Interceptor.attach(Module.findExportByName(null, "GdipSetSmoothingMode"), {
    onEnter: function(args) {
        counter++;
        //console.log("GdipSetSmoothingMode called: " + counter);
    }
});


Interceptor.attach(Module.findExportByName(null, "GdipCreatePen1"), {
    onEnter: function(args) {
        // Check if the first argument is 0xFF000000
        if (!this.context.rbx.equals(0xFF000000)) {
            // console.log("rbx != 0xFF000000, skipping...");
            // Modify the return address to skip the function execution
            args[0] = ptr(0);
            args[1] = ptr(0);
            args[2] = ptr(0);
            args[3] = ptr(0);
        } else {
            //console.log("GdipCreatePen1 called: " + (this.returnAddress.sub(base).toString(16)));
            var ret_addr = this.returnAddress.sub(base);
            var str = "";

            /*0 -> a252 맞음
            1 -> b5c3 맞음
            2 -> ca7e 맞음
            3 -> df2c 맞음
            4 -> f36e 맞음
            5 -> 1081f 맞음
            6 -> 11d4d 맞음
            7 -> 1318b 맞음
            8 -> 14723 맞음
            9 -> 15c38 맞음

            a -> 26dd 맞음
            b -> 3b95
            c -> 4fe0 맞음
            d -> 6424 맞음
            e -> 78e6 맞음
            f -> 8d38 맞음*/

            if (ret_addr == 0xa252)
                str = "0";
            else if (ret_addr == 0xb5c3)
                str = "1";
            else if (ret_addr == 0xca7e)
                str = "2";
            else if (ret_addr == 0xdf2c)
                str = "3";
            else if (ret_addr == 0xf36e)
                str = "4";
            else if (ret_addr == 0x1081f)
                str = "5";
            else if (ret_addr == 0x11d4d)
                str = "6";
            else if (ret_addr == 0x1318b)
                str = "7";
            else if (ret_addr == 0x14723)
                str = "8";
            else if (ret_addr == 0x15c38)
                str = "9";
            else if (ret_addr == 0x26dd)
                str = "a";
            else if (ret_addr == 0x3b95)
                str = "b";
            else if (ret_addr == 0x4fe0)
                str = "c";
            else if (ret_addr == 0x6424)
                str = "d";
            else if (ret_addr == 0x78e6)
                str = "e";
            else if (ret_addr == 0x8d38)
                str = "f";
            else
                str = "unknown";   
            if (str != "unknown")
            {
                file.write(str);
                console.log(str);
            }


        }
    }
});

"""

def main(target_process):
    # Attach to the process
    session = frida.attach(target_process)
    
    # Create a script with the provided JavaScript code
    script = session.create_script(jscode)
    script.on('message', on_message)
    
    # Load the script into the process
    script.load()
    
    # Keep the Python script running until interrupted
    print("Press Ctrl+C to stop.")
    try:
        sys.stdin.read()
    except KeyboardInterrupt:
        pass

    # Detach the session
    session.detach()

if __name__ == "__main__":
    main("CaptainHook.exe")