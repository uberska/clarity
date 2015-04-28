//
//  MasterViewController.swift
//  clarity
//
//  Created by Scott Douglas on 4/12/15.
//  Copyright (c) 2015 Scott Douglas. All rights reserved.
//

import Cocoa
import Darwin
import Foundation


class MasterViewController: NSViewController {    
    @IBOutlet weak var tableView: NSTableView!

    @IBOutlet var textView: NSTextView!
    
    @IBOutlet weak var splitView: NSSplitView!

    var diffables: [Diffable] = []
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        diffables += [FileDiffable(onePath: "/git/clarity_test_files/same_one.txt", twoPath: "/git/clarity_test_files/same_two.txt")]
        diffables += [FileDiffable(onePath: "/git/clarity_test_files/modified_one.txt", twoPath: "/git/clarity_test_files/modified_two.txt")]
        diffables += [FileDiffable(onePath: "/git/clarity_test_files/removed_one.txt", twoPath: "/git/clarity_test_files/removed_two.txt")]
        diffables += [FileDiffable(onePath: "/git/clarity_test_files/added_one.txt", twoPath: "/git/clarity_test_files/added_two.txt")]
        
        clearDiffView()
        
        var arguments: [String] = Array(Process.arguments[1..<Process.arguments.count])
        
        // Remove XCode arguments
        arguments = arguments.filter {
            $0 != "-NSDocumentRevisionsDebugMode" && $0 != "YES"
        }

        if arguments.count > 0 {
            diffables.removeAll(keepCapacity: false)

            var index = 0
            for ; index < arguments.count - 1; index = index + 2 {
                let onePath = arguments[index]
                let twoPath = arguments[index + 1]
                diffables.append(FileDiffable(onePath: onePath, twoPath: twoPath))
            }
            
            if index == arguments.count - 1 {
                println("WARNING: Skipping file '\(arguments[index])' since it's an odd file and there is nothing left in the arguments to diff it against.")
            }
        }
        
        println(diffables)
    }
    
    override func viewDidAppear() {
        splitView.setPosition(200.0, ofDividerAtIndex: 0)
        
        tableView.selectRowIndexes(NSIndexSet(index: 0), byExtendingSelection: false)
    }
    
    func clearDiffView() {
        textView.textStorage?.mutableString.setString("")
        //textView.append("No Files Loaded.")
    }
    
    func selectedDiffable() -> Diffable? {
        let selectedRow = self.tableView.selectedRow;
        if selectedRow >= 0 && selectedRow < self.diffables.count {
            return self.diffables[selectedRow]
        }
        return nil
    }
    
    func updateDiffView(diffable: Diffable?) {
        if let diffable = diffable {
            diffable.updateView(textView)
        } else {
            clearDiffView()
        }
    }
}


extension MasterViewController: NSTableViewDataSource {
    func numberOfRowsInTableView(aTableView: NSTableView) -> Int {
        return self.diffables.count
    }
    
    func tableView(tableView: NSTableView, viewForTableColumn tableColumn: NSTableColumn?, row: Int) -> NSView? {
        var cellView: NSTableCellView = tableView.makeViewWithIdentifier(tableColumn!.identifier, owner: self) as! NSTableCellView
        
        if tableColumn!.identifier == "Diffable" {
            let diffable: Diffable = self.diffables[row]
            cellView.textField!.stringValue = diffable.getDisplayString()
            return cellView
        }
        
        return cellView
    }
}


extension MasterViewController: NSTableViewDelegate {
    func tableViewSelectionDidChange(notification: NSNotification) {
        let diffable = selectedDiffable()
        updateDiffView(diffable)
    }
}


extension NSTextView {
    func append(string: String, backgroundColor: NSColor = NSColor.whiteColor()) {
//        var attributes:NSDictionary = [
//            NSBackgroundColorAttributeName: NSColor.redColor()
//        ]
        
        self.textStorage?.appendAttributedString(NSAttributedString(string: string, attributes: [NSBackgroundColorAttributeName: backgroundColor]))

        
//        var attributedString = NSMutableAttributedString(string: string);
//        attributedString.addAttribute(NSBackgroundColorAttributeName, value: NSColor.redColor());
//        self.textStorage?.appendAttributedString(attributedString)
    }
}