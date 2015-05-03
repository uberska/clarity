//
//  clarityTests.swift
//  clarityTests
//
//  Created by Scott Douglas on 4/12/15.
//  Copyright (c) 2015 Scott Douglas. All rights reserved.
//

import Cocoa
import XCTest

class clarityTests: XCTestCase {
    
    override func setUp() {
        super.setUp()
        // Put setup code here. This method is called before the invocation of each test method in the class.
    }
    
    override func tearDown() {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        super.tearDown()
    }
    
    func testParseTextRange() {
        // Single numbers
        XCTAssert(parseTextRange("0") == Range<Int>(start: -1, end: 0))
        XCTAssert(parseTextRange("1") == Range<Int>(start: 0, end: 1))
        XCTAssert(parseTextRange("3") == Range<Int>(start: 2, end: 3))
        XCTAssert(parseTextRange("14") == Range<Int>(start: 13, end: 14))
        
        // Ranges
        XCTAssert(parseTextRange("0,0") == Range<Int>(start: -1, end: 0))
        XCTAssert(parseTextRange("0,1") == Range<Int>(start: -1, end: 1))
        XCTAssert(parseTextRange("0,4") == Range<Int>(start: -1, end: 4))
        XCTAssert(parseTextRange("2,2") == Range<Int>(start: 1, end: 2))
        XCTAssert(parseTextRange("2,3") == Range<Int>(start: 1, end: 3))
        XCTAssert(parseTextRange("2,7") == Range<Int>(start: 1, end: 7))
        XCTAssert(parseTextRange("43,47") == Range<Int>(start: 42, end: 47))
    }

    func testParseDiffLine() {
        let chunk: TextDiffModelChunk = parseTextDiffLine(TextDiffModelChunk.Status.Modified, "c", "1c2")
        
        XCTAssert(chunk.status == TextDiffModelChunk.Status.Modified)
        XCTAssert(chunk.oneRange == Range<Int>(start: 0, end: 1))
        XCTAssert(chunk.twoRange == Range<Int>(start: 1, end: 2))
    }
    
    func testSources() {
        let oneContents = "The contents for one."
        let twoContents = "The contents for two."
        
        let oneName = "One name."
        let twoName = "Two name."

        let diffModel = diffTextStrings(oneName, oneContents, twoName, twoContents)
        
        XCTAssert(diffModel.one!.name == oneName)
        XCTAssert(diffModel.one!.contentsAsString() == oneContents)
        
        XCTAssert(diffModel.two!.name == twoName)
        XCTAssert(diffModel.two!.contentsAsString() == twoContents)
    }
    
    func testSimpleSame() {
        let theSameContents = "The same contents."
        let diffModel = diffTextStrings("one", theSameContents, "two", theSameContents)

        XCTAssert(diffModel.chunks.count == 1)
        XCTAssert(diffModel.chunks[0].status == TextDiffModelChunk.Status.Same)
        XCTAssert(diffModel.chunks[0].oneRange == Range<Int>(start: 0, end: 1))
        XCTAssert(diffModel.chunks[0].twoRange == Range<Int>(start: 0, end: 1))
    }
    
    func testSimpleAdded() {
        let diffModel = diffTextStrings("one", "", "two", "Added.")
        
        XCTAssert(diffModel.chunks.count == 1)
        XCTAssert(diffModel.chunks[0].status == TextDiffModelChunk.Status.Added)
        XCTAssert(diffModel.chunks[0].oneRange == Range<Int>(start: -1, end: 0))
        XCTAssert(diffModel.chunks[0].twoRange == Range<Int>(start: 0, end: 1))
    }

    func testSimpleRemoved() {
        let diffModel = diffTextStrings("one", "Removed.", "two", "")
        
        XCTAssert(diffModel.chunks.count == 1)
        XCTAssert(diffModel.chunks[0].status == TextDiffModelChunk.Status.Removed)
        XCTAssert(diffModel.chunks[0].oneRange == Range<Int>(start: 0, end: 1))
        XCTAssert(diffModel.chunks[0].twoRange == Range<Int>(start: -1, end: 0))
    }
    
    func testSimpleModified() {
        let diffModel = diffTextStrings("one", "Modified one.", "two", "Modified two.")
        
        XCTAssert(diffModel.chunks.count == 1)
        XCTAssert(diffModel.chunks[0].status == TextDiffModelChunk.Status.Modified)
        XCTAssert(diffModel.chunks[0].oneRange == Range<Int>(start: 0, end: 1))
        XCTAssert(diffModel.chunks[0].twoRange == Range<Int>(start: 0, end: 1))
    }
    
    func testSameAtBeginningOfModified() {
        let diffModel = diffTextStrings("one", "Same\nSame\nModified one.", "two", "Same\nSame\nModified two.")
        
        XCTAssert(diffModel.chunks.count == 2)

        XCTAssert(diffModel.chunks[0].status == TextDiffModelChunk.Status.Same)
        XCTAssert(diffModel.chunks[0].oneRange == Range<Int>(start: 0, end: 2))
        XCTAssert(diffModel.chunks[0].twoRange == Range<Int>(start: 0, end: 2))

        XCTAssert(diffModel.chunks[1].status == TextDiffModelChunk.Status.Modified)
        XCTAssert(diffModel.chunks[1].oneRange == Range<Int>(start: 2, end: 3))
        XCTAssert(diffModel.chunks[1].twoRange == Range<Int>(start: 2, end: 3))
    }
    
    func testSameAtEndOfModified() {
        let diffModel = diffTextStrings("one", "Modified one.\nSame\nSame", "two", "Modified two.\nSame\nSame")
        
        XCTAssert(diffModel.chunks.count == 2)

        XCTAssert(diffModel.chunks[0].status == TextDiffModelChunk.Status.Modified)
        XCTAssert(diffModel.chunks[0].oneRange == Range<Int>(start: 0, end: 1))
        XCTAssert(diffModel.chunks[0].twoRange == Range<Int>(start: 0, end: 1))

        XCTAssert(diffModel.chunks[1].status == TextDiffModelChunk.Status.Same)
        XCTAssert(diffModel.chunks[1].oneRange == Range<Int>(start: 1, end: 3))
        XCTAssert(diffModel.chunks[1].twoRange == Range<Int>(start: 1, end: 3))
    }
	
    func testSameAtBeginningOfAdded() {
        let diffModel = diffTextStrings("one", "Same\n", "two", "Same\nAdded.\n")
        
        XCTAssert(diffModel.chunks.count == 2)
        
        XCTAssert(diffModel.chunks[0].status == TextDiffModelChunk.Status.Same)
        XCTAssert(diffModel.chunks[0].oneRange == Range<Int>(start: 0, end: 1))
        XCTAssert(diffModel.chunks[0].twoRange == Range<Int>(start: 0, end: 1))
        
        XCTAssert(diffModel.chunks[1].status == TextDiffModelChunk.Status.Added)
        XCTAssert(diffModel.chunks[1].oneRange == Range<Int>(start: 1, end: 2))
        XCTAssert(diffModel.chunks[1].twoRange == Range<Int>(start: 1, end: 2))
    }
	
	/*
    func testSameAtEndOfAdded() {
        let diffModel = diffTextStrings("one", "Same\nSame", "two", "Added.\nSame\nSame")
        
        XCTAssert(diffModel.chunks.count == 2)
        
        XCTAssert(diffModel.chunks[0].status == TextDiffModelChunk.Status.Added)
        XCTAssert(diffModel.chunks[0].oneRange == Range<Int>(start: -1, end: 0))
        XCTAssert(diffModel.chunks[0].twoRange == Range<Int>(start: 0, end: 1))
        
        XCTAssert(diffModel.chunks[1].status == TextDiffModelChunk.Status.Same)
        XCTAssert(diffModel.chunks[1].oneRange == Range<Int>(start: 1, end: 3))
        XCTAssert(diffModel.chunks[1].twoRange == Range<Int>(start: 1, end: 3))
    }
    */
}
