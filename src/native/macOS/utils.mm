#include "../utils.h"

#include <iostream>
#include <string>
#include <CoreFoundation/CoreFoundation.h>
#include <Cocoa/Cocoa.h>

namespace eokas {
    bool OpenFileDialog(String& selectedPath, const std::map<String, String>& filters) {
        @autoreleasepool {
            NSOpenPanel *panel = [NSOpenPanel openPanel];
            panel.message = @"Open";
            panel.prompt = @"OK";
            panel.canChooseFiles = YES;
            panel.allowsMultipleSelection = NO;
            panel.allowedFileTypes = [NSArray arrayWithObjects:@"txt", @"cpp", @"h", nil];
            
            if ([panel runModal] == NSModalResponseOK) {
                NSURL *fileURL = [[panel URLs] objectAtIndex:0];
                selectedPath = [[fileURL path] UTF8String];
                return true;
            }
        }
        return false;
    }
    
    bool OpenDirectoryDialog(String& selectedPath, const String& defaultPath) {
        @autoreleasepool {
            NSOpenPanel *panel = [NSOpenPanel openPanel];
            panel.message = @"Open";
            panel.prompt = @"OK";
            panel.canChooseFiles = NO;
            panel.allowsMultipleSelection = NO;
            panel.allowedFileTypes = [NSArray arrayWithObjects:@"txt", @"cpp", @"h", nil];
            
            if ([panel runModal] == NSModalResponseOK) {
                NSURL *fileURL = [[panel URLs] objectAtIndex:0];
                selectedPath = [[fileURL path] UTF8String];
                return true;
            }
        }
        return false;
    }
}
