//
//  AppDelegate.swift
//  clarity
//
//  Created by Scott Douglas on 4/12/15.
//  Copyright (c) 2015 Scott Douglas. All rights reserved.
//

import Cocoa


public class ClarityHelpCommand: HelpCommand {
	
	override public func showHelpOnHFlag() -> Bool  {
		return true
	}
	
	override public func failOnUnrecognizedOptions() -> Bool  {
		return false
	}
	
	override public func execute() -> ExecutionResult  {
		super.execute()
		exit(0)
	}
	
}


@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {

    @IBOutlet weak var window: NSWindow!
	
    var masterViewController: MasterViewController!
	
	func applicationWillFinishLaunching(notification: NSNotification) {
		CLI.setup(name: "clarity", version: "0.1", description: "clarity - diff tool")
		CLI.registerCustomHelpCommand(ClarityHelpCommand())
		var result = CLI.go()
	}

    func applicationDidFinishLaunching(aNotification: NSNotification) {
        masterViewController = MasterViewController(nibName: "MasterViewController", bundle: nil)
        
        window.contentView.addSubview(masterViewController.view)
        masterViewController.view.frame = (window.contentView as! NSView).bounds
        
        masterViewController.view.translatesAutoresizingMaskIntoConstraints = false
        let verticalConstraints = NSLayoutConstraint.constraintsWithVisualFormat("V:|[subView]|",
            options: NSLayoutFormatOptions(0),
            metrics: nil,
            views: ["subView" : masterViewController.view])
        let horizontalConstraints = NSLayoutConstraint.constraintsWithVisualFormat("H:|[subView]|",
            options: NSLayoutFormatOptions(0),
            metrics: nil,
            views: ["subView" : masterViewController.view])
        
        NSLayoutConstraint.activateConstraints(verticalConstraints + horizontalConstraints)
    }

    func applicationWillTerminate(aNotification: NSNotification) {
        // Insert code here to tear down your application
    }
    
    func applicationShouldTerminateAfterLastWindowClosed(sender: NSApplication) -> Bool {
        return true
    }
}

