//
//  TextDiff.swift
//  clarity
//
//  Created by Scott Douglas on 4/15/15.
//  Copyright (c) 2015 Scott Douglas. All rights reserved.
//


import Cocoa
import DiffMatchPatch
import Foundation


class TextDiffModelChunk {
    enum Status {
        case Same, Added, Removed, Modified
    }
    
    var status: Status;
    var oneRange: Range<Int>;
    var twoRange: Range<Int>;
    
    func statusString(status: Status) -> String {
        switch status {
        case .Same:
            return "Same"
        case .Added:
            return "Added"
        case .Modified:
            return "Modified"
        case .Removed:
            return "Removed"
        default:
            return "Unknown"
        }
    }
    
    init(status: Status, oneRange: Range<Int>, twoRange: Range<Int>) {
        self.status = status;
        self.oneRange = oneRange;
        self.twoRange = twoRange;
    }
}


class TextDiffSource {
    var name: String
    var contents: [String]
    var file: String
    
    init(name: String, contents: [String], file: String) {
        self.name = name
        
        self.contents = contents
        
        self.file = file
    }
    
    func contentsAsString() -> String {
        return "\n".join(contents)
    }
}


class TextDiffModel {
    var one: TextDiffSource?
    var two: TextDiffSource?

    var chunks: [TextDiffModelChunk]

    init() {
        self.chunks = [TextDiffModelChunk]()
    }

    init(one: TextDiffSource, two: TextDiffSource, chunks: [TextDiffModelChunk]) {
        self.one = one
        self.two = two

        self.chunks = chunks
    }
}


func diffTextFromPaths(onePath: String, twoPath: String) -> TextDiffModel? {
    let oneContents = String(contentsOfFile: onePath, encoding: NSUTF8StringEncoding, error: nil)
    let twoContents = String(contentsOfFile: twoPath, encoding: NSUTF8StringEncoding, error: nil)
    
    if let oneContents = oneContents {
        if let twoContents = twoContents {
            var oneSource = TextDiffSource(name: onePath, contents: oneContents.componentsSeparatedByString("\n"), file: onePath)
            var twoSource = TextDiffSource(name: twoPath, contents: twoContents.componentsSeparatedByString("\n"), file: twoPath)
            
            return diffTextSources(oneSource, twoSource)
        }
    }
    
    return nil
}


func diffTextStrings(oneName: String, oneContents: String, twoName: String, twoContents: String) -> TextDiffModel {
    // Write to temp file. This is not good since it will be stomped on with two calls to
    // this function. TODO: Fix this.
    var oneTempFile = NSTemporaryDirectory().stringByAppendingPathComponent("one.txt")
    var twoTempFile = NSTemporaryDirectory().stringByAppendingPathComponent("two.txt")
    
    oneContents.writeToFile(oneTempFile, atomically: false, encoding: NSUTF8StringEncoding, error: nil);
    twoContents.writeToFile(twoTempFile, atomically: false, encoding: NSUTF8StringEncoding, error: nil);

    var oneSource = TextDiffSource(name: oneName, contents: oneContents.componentsSeparatedByString("\n"), file: oneTempFile)
    var twoSource = TextDiffSource(name: twoName, contents: twoContents.componentsSeparatedByString("\n"), file: twoTempFile)
    
    return diffTextSources(oneSource, twoSource)
}


func parseTextRange(text: String) -> Range<Int> {
    println("Parsing text range \(text)")
    if text.rangeOfString(",") != nil {
        let delimiterIndex: Range<String.Index> = text.rangeOfString(",")!
        
        let startStringRange = Range<String.Index>(start: text.startIndex, end: delimiterIndex.startIndex)
        let endStringRange = Range<String.Index>(start: delimiterIndex.endIndex, end: text.endIndex)
        
        let startString: String = text.substringWithRange(startStringRange)
        let endString: String = text.substringWithRange(endStringRange)
        
        return Range<Int>(
            start: startString.toInt()! - 1,
            end: endString.toInt()!
        )
    } else {
        let index = text.toInt()! - 1
        return Range<Int>(start: index, end: index + 1)
    }
}


func parseTextDiffLine(status: TextDiffModelChunk.Status, delimiter: String, line: String) -> TextDiffModelChunk {
    println("####")
    println(line)
    println(delimiter)
    let index = line.rangeOfString(delimiter)
    println(index)
    let oneRange = parseTextRange(line.substringToIndex(index!.startIndex))
    println(oneRange)
    let twoRange = parseTextRange(line.substringFromIndex(index!.endIndex))
    println(twoRange)
    return TextDiffModelChunk(status: status, oneRange: oneRange, twoRange: twoRange)
}


func parseTextDiff(model: TextDiffModel, diffLines: [String]) {
    // Check if the files are exactly the same
    if diffLines.count == 0 {
        let fullRange: Range<Int> = 0...diffLines.count
        let sameChunk = TextDiffModelChunk(status: .Same, oneRange: fullRange, twoRange: fullRange)
        model.chunks.append(sameChunk)
        return
    }
    
    var lastOneEndIndex = -1
    
    for diffLine in diffLines {
        if diffLine.rangeOfString("c") != nil {
            let chunk: TextDiffModelChunk = parseTextDiffLine(TextDiffModelChunk.Status.Modified, "c", diffLine)
            
            if (chunk.oneRange.startIndex - lastOneEndIndex > 1) {
                let oneRange = Range<Int>(
                    start: lastOneEndIndex + 1,
                    end: chunk.oneRange.startIndex
                )
                let twoRange = Range<Int>(
                    start: chunk.twoRange.startIndex - (oneRange.endIndex - oneRange.startIndex),
                    end: chunk.twoRange.startIndex
                )
                let sameChunk = TextDiffModelChunk(status: TextDiffModelChunk.Status.Same, oneRange: oneRange, twoRange: twoRange)

                model.chunks.append(sameChunk)
            }
            
            model.chunks.append(chunk)
            
            lastOneEndIndex = chunk.oneRange.endIndex - 1
        } else if diffLine.rangeOfString("a") != nil {
            let chunk: TextDiffModelChunk = parseTextDiffLine(TextDiffModelChunk.Status.Added, "a", diffLine)

            if chunk.oneRange.startIndex - lastOneEndIndex > 1 {
                let oneRange = Range<Int>(
                    start: lastOneEndIndex + 1,
                    end: chunk.oneRange.startIndex
                )
                let twoRange = Range<Int>(
                    start: chunk.twoRange.startIndex - (oneRange.endIndex - oneRange.startIndex),
                    end: chunk.twoRange.startIndex
                )
                let sameChunk = TextDiffModelChunk(status: TextDiffModelChunk.Status.Same, oneRange: oneRange, twoRange: twoRange)
                
                model.chunks.append(sameChunk)
            }
            
            model.chunks.append(chunk)
            
            lastOneEndIndex = chunk.oneRange.endIndex - 1
        } else if diffLine.rangeOfString("d") != nil {
            let chunk: TextDiffModelChunk = parseTextDiffLine(TextDiffModelChunk.Status.Removed, "d", diffLine)
            
            if chunk.oneRange.startIndex - lastOneEndIndex > 1 {
                let oneRange = Range<Int>(
                    start: lastOneEndIndex + 1,
                    end: chunk.oneRange.startIndex
                )
                let twoRange = Range<Int>(
                    start: chunk.twoRange.startIndex - (oneRange.endIndex - oneRange.startIndex) + 1,
                    end: chunk.twoRange.startIndex + 1
                )
                /* TODO: note extra plus ones when combining in method */
                /* numbers in range of removed is what line the removed line is after,
                not which line number is the removed line */
                let sameChunk = TextDiffModelChunk(status: TextDiffModelChunk.Status.Same, oneRange: oneRange, twoRange: twoRange)
                
                model.chunks.append(sameChunk)
            }
            
            model.chunks.append(chunk)
            
            lastOneEndIndex = chunk.oneRange.endIndex - 1
        } else {
            assertionFailure("WARNING: Unknown diff line: \(diffLine)")
        }
    }
    

    // Check if there are lines after the final diff command that are 'Same' lines.
    // Don't add these if the first file is an empty string. This is a special case
    // where anything in the second file is considered an 'Added' so no 'Same' lines
    // should be added.
    let contentExistsAfterLastDiffLine = lastOneEndIndex < model.one!.contents.count - 1
    let oneContentsHasContent = model.one!.contentsAsString() != ""

    if contentExistsAfterLastDiffLine && oneContentsHasContent {
        let lastChunk: TextDiffModelChunk = model.chunks.last!

        let oneRange = Range<Int>(
            start: lastChunk.oneRange.endIndex,
            end: model.one!.contents.count
        )

        let twoRange = Range<Int>(
            start: lastChunk.twoRange.endIndex,
            end: model.two!.contents.count
        )

        assert(oneRange.endIndex - oneRange.startIndex == twoRange.endIndex - twoRange.startIndex)
        
        let sameChunk = TextDiffModelChunk(status: TextDiffModelChunk.Status.Same, oneRange: oneRange, twoRange: twoRange)
        
        model.chunks.append(sameChunk)
    }
}


func isDiffLine(line: String) -> Bool {
    if count(line) > 1 {
        let firstChar = line.substringWithRange(Range<String.Index>(start: line.startIndex, end: line.startIndex.successor()))
        return firstChar.toInt() != nil
    }
    
    return false
}


func diffTextSources(one: TextDiffSource, two: TextDiffSource) -> TextDiffModel {
    var model = TextDiffModel();

    var diffOutput = run("diff \(one.file) \(two.file)")
    
    var diffLines: [String] = diffOutput.componentsSeparatedByString("\n")

    diffLines = diffLines.filter { isDiffLine($0) }
    
    model.one = one;
    model.two = two;

    parseTextDiff(model, diffLines)

    return model;
}


func updateFromModelUnified(textView: NSTextView, model: TextDiffModel) {
    // Clear out the existing NSTextView
    if let textStorage = textView.textStorage {
        textStorage.mutableString.setString("")
    }
    
    
    // Add our diff text
    if model.one != nil && model.two != nil {
        let oneContents = model.one!.contents
        let twoContents = model.two!.contents
    
        for chunk in model.chunks {
            switch chunk.status {
            case .Same:
                addTextViewUnifiedChunk(textView, oneContents, chunk.oneRange, DiffColors.sameBackground)
            case .Removed:
                addTextViewUnifiedChunk(textView, oneContents, chunk.oneRange, DiffColors.removedBackground)
            case .Added:
                addTextViewUnifiedChunk(textView, twoContents, chunk.twoRange, DiffColors.addedBackground)
            case .Modified:
				let oneStr = "\n".join(oneContents[chunk.oneRange]) + "\n"
				let twoStr = "\n".join(twoContents[chunk.twoRange]) + "\n"
				
				let diffMatchPatch = DiffMatchPatch()
				let diffs: NSMutableArray! = diffMatchPatch.diff_mainOfOldString(oneStr, andNewString: twoStr)

				addTextViewUnifiedChunkDiff(textView, diffs, DiffColors.modifiedOneBackground, DiffColors.removedBackground, DIFF_INSERT)
                addTextViewUnifiedChunkDiff(textView, diffs, DiffColors.modifiedTwoBackground, DiffColors.addedBackground, DIFF_DELETE)
            }
        }
    }
	

    // TODO: move the cursor to the beginning of the file
}


func addTextViewUnifiedChunkDiff(textView: NSTextView, diffs: NSMutableArray, sameColor: NSColor, differentColor: NSColor, skipOperation: Operation) {
	for diff in diffs {
		let diffOperation: Operation = diff.operation!
		if diffOperation != skipOperation {
			if diffOperation == DIFF_INSERT || diffOperation == DIFF_DELETE {
				textView.append(diff.text, backgroundColor: differentColor)
			} else if diffOperation == DIFF_EQUAL {
				textView.append(diff.text, backgroundColor: sameColor)
			}
		}
	}
}


public func ==(lhs: Operation, rhs: Operation) -> Bool {
	var leftValue: UInt32 = unsafeBitCast(lhs, UInt32.self)
	var rightValue: UInt32 = unsafeBitCast(rhs, UInt32.self)
	
	return leftValue == rightValue
}


public func !=(lhs: Operation, rhs: Operation) -> Bool {
	return !(lhs == rhs)
}


func addTextViewUnifiedChunk(textView: NSTextView, contents: [String], range: Range<Int>, backgroundColor: NSColor) {
    for index in range {
        textView.append(contents[index] + "\n", backgroundColor: backgroundColor)
    }
}
