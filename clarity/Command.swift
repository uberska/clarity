//
//  Command.swift
//  clarity
//
//  Created by Scott Douglas on 4/15/15.
//  Copyright (c) 2015 Scott Douglas. All rights reserved.
//

import Foundation


func run(cmd: String) -> String {
    let chunks = cmd.componentsSeparatedByString(" ")

    let task = NSTask()
    task.launchPath = "/usr/bin/env"
    task.arguments = chunks

    let pipe = NSPipe()
    task.standardOutput = pipe
    task.launch()
    
    let data = pipe.fileHandleForReading.readDataToEndOfFile()
    let output: String = NSString(data: data, encoding: NSUTF8StringEncoding) as! String
    
    // task.terminationStatus
    
    return output
}
