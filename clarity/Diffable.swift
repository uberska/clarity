//
//  Diffable.swift
//  clarity
//
//  Created by Scott Douglas on 4/22/15.
//  Copyright (c) 2015 Scott Douglas. All rights reserved.
//

import Cocoa


class Diffable {
    func getDisplayString() -> String {
        fatalError("Subclasses must override this method.")
    }
    
    func updateView(textView: NSTextView) {
        fatalError("Subclasses must override this method.")
    }
}


class FileDiffable : Diffable {
    var onePath: String
    var twoPath: String
    
    init(onePath: String, twoPath: String) {
        self.onePath = onePath
        self.twoPath = twoPath
    }
    
    override func getDisplayString() -> String {
        return "\(onePath) -> \(twoPath)"
    }
    
    override func updateView(textView: NSTextView) {
        let cmd = "diff \(onePath) \(twoPath)"
        println(cmd)
        let cmdOutput = run(cmd)
        println(cmdOutput)
        
        let diffModel = diffTextFromPaths(onePath, twoPath)
        
        if let diffModel = diffModel {
            updateFromModelUnified(textView, diffModel)
        }
    }
}
