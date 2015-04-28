//
//  Colors.swift
//  clarity
//
//  Created by Scott Douglas on 4/17/15.
//  Copyright (c) 2015 Scott Douglas. All rights reserved.
//


import Cocoa


class DiffColors {
    class var sameBackground: NSColor { return NSColor(red: 1.0, green: 1.0, blue: 1.0, alpha: 1.0) }
    class var addedBackground: NSColor { return NSColor(red: 0.0, green: 150.0 / 255.0, blue: 110.0 / 255.0, alpha: 1.0) }
    class var removedBackground: NSColor { return NSColor(red: 220.0 / 255.0, green: 0.0, blue: 0.0, alpha: 1.0) }
    class var modifiedOneBackground: NSColor { return NSColor(red: 245.0 / 255.0, green: 180.0 / 255.0, blue: 180.0 / 255.0, alpha: 1.0) }
    class var modifiedTwoBackground: NSColor { return NSColor(red: 175.0 / 255.0, green: 225.0 / 255.0, blue: 210.0 / 255.0, alpha: 1.0) }
}
