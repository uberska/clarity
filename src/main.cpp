#include <iostream>
#include <limits>
#include <vector>

// TODO: remove this
#include "src/git/repository.h"
#include <git2.h>

#include <boost/program_options/errors.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <QApplication>
#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QtCore>
#include <QSplitter>
#include <QString>
#include <QLabel>
#include <QList>
#include <QMap>
#include <QVariant>

#include "src/diff_controller.h"
#include "src/diff_match_patch/diff_match_patch.h"
#include "src/command.h"
#include "src/diff_model_text.h"
#include "src/diff_selector_widget.h"
#include "src/diff_text.h"
#include "src/diff_text_edit.h"
#include "src/file_diffable.h"
#include "src/git_diffable.h"
#include "src/strings.h"


namespace po = boost::program_options;


void testTextDiff() {
	std::cout << "Testing diff..." << std::endl;

	const char* one = "/home/uberska/clarity/one.txt";
	const char* two = "/home/uberska/clarity/two.txt";

	DiffModelText model;
	diffTextFromFiles(model, one, two);

	std::cout << model << std::endl;
}


void testDiffMatchPatch() {
	std::cout << "Starting..." << std::endl;

	diff_match_patch dmp;
	QString str1 = QString("string in\n diff");
	QString str2 = QString("string in diff added");

	clock_t deadline = std::numeric_limits<clock_t>::max();

	QList<Diff> diffs = dmp.diff_lineMode(str1, str2, deadline);
	QString html = dmp.diff_prettyHtml(diffs);
	std::cout << "HTML BEGIN" << std::endl;
	std::cout << html.toUtf8().constData() << std::endl;
	std::cout << "HTML END" << std::endl;
	QString delta = dmp.diff_toDelta(diffs);
	std::cout << "DELTA END" << std::endl;
	std::cout << delta.toUtf8().constData() << std::endl;
	std::cout << "DELTA END" << std::endl;

	QString strPatch = dmp.patch_toText(dmp.patch_make(str1, str2));
	QList<Patch> patchList(dmp.patch_fromText(strPatch));
	QPair<QString, QVector<bool> > out
		= dmp.patch_apply(patchList, str1);
	QString strResult = out.first;

	std::cout << strResult.toUtf8().constData() << std::endl;

	std::cout << "Ending..." << std::endl;

	// here, strResult will equal str2 above.
}


void printGitDiffFile(const git_diff_file& file) {
	char oidStr[41];
	oidStr[40] = '\0';
	git_oid_fmt(oidStr, &file.oid);

	std::cout << "[git_diff_file oid='" << oidStr <<
		"' path='" << file.path <<
		"' size='" << file.size <<
		"' flags='" << file.flags <<
		"' mode='" << file.mode <<
		"']";
}


// TODO: remove this if it's not needed
int oldGitCode() {
	// TODO: check repository state before doing anything git_repository_state

	int error = 0;

	git_repository *repo = NULL;
	git_repository_open(&repo, "/home/uberska/clarity/.git");

	git_reference *head = NULL;
	error = git_repository_head(&head, repo);

	if (error != 0 ) {
		std::cerr << "\nError getting repository head.\n";
		return EXIT_FAILURE;
	}

	git_ref_t type = git_reference_type(head);

	char out[41];
	out[40] = '\0';

	if (type == GIT_REF_OID) {
		std::cout << "\nOID found.\n";
		git_oid_fmt(out, git_reference_target(head));
		std::cout << "[" << out << "]\n";

		const char* branchName;
		error = git_branch_name(&branchName, head);
		if (error != 0 ) {
			std::cerr << "\nError getting branch name.\n";
			return EXIT_FAILURE;
		}
		std::cout << "[" << branchName << "]\n";
	} else if (type == GIT_REF_SYMBOLIC) {
		std::cout << "\nSymbolic found.\n";
	}




	/*
	git_index* index = index;
	error = git_repository_index(&index, repo);
	if (error != 0 ) {
		std::cerr << "\nError getting repository index.\n";
		return EXIT_FAILURE;
	}

	size_t indexCount = git_index_entrycount(index);
	for (unsigned int i = 0; i < indexCount; i++) {
		const git_index_entry* indexEntry = git_index_get_byindex(index, i);

		git_oid_fmt(out, &indexEntry->oid);

		printf("File Path: %s\n", indexEntry->path);
		printf("    Stage: %d\n", git_index_entry_stage(indexEntry));
		printf(" Blob SHA: %s\n", out);
		printf("File Mode: %07o\n", indexEntry->mode);
		printf("File Size: %d bytes\n", (int)indexEntry->file_size);
		printf("Dev/Inode: %d/%d\n", (int)indexEntry->dev, (int)indexEntry->ino);
		printf("  UID/GID: %d/%d\n", (int)indexEntry->uid, (int)indexEntry->gid);
		printf("    ctime: %d\n", (int)indexEntry->ctime.seconds);
		printf("    mtime: %d\n", (int)indexEntry->mtime.seconds);
		printf("\n");
	}
	*/




	git_status_list* status = NULL;
	git_status_options statusOptions = GIT_STATUS_OPTIONS_INIT;
	statusOptions.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
	statusOptions.flags = 
		GIT_STATUS_OPT_INCLUDE_UNTRACKED |
		GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS |
		GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
		GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR |
		GIT_STATUS_OPT_SORT_CASE_SENSITIVELY |
		GIT_STATUS_OPT_RENAMES_FROM_REWRITES;

	error = git_status_list_new(&status, repo, &statusOptions);
	if (error != 0 ) {
		std::cerr << "\nError getting repository status.\n";
		return EXIT_FAILURE;
	}

	size_t statusCount = git_status_list_entrycount(status);
	for (unsigned int i = 0; i < statusCount; i++) {
		const git_status_entry* entry = git_status_byindex(status, i);

		//std::cout << ">>> ENTRY - " << getStatusStr(entry->status) << "\n";
		std::cout << ">>> ENTRY - " << entry->status << "\n";

		if (entry->head_to_index) {
			std::cout << "\nOLD:\n";
			printGitDiffFile(entry->head_to_index->old_file);
			std::cout << "\nNEW:\n";
			printGitDiffFile(entry->head_to_index->new_file);
			std::cout << "\n";
		} else {
			std::cout << "\nNo head_to_index.\n";
		}

		if (entry->index_to_workdir) {
			std::cout << "\nOLD:\n";
			printGitDiffFile(entry->index_to_workdir->old_file);
			std::cout << "\nNEW:\n";
			printGitDiffFile(entry->index_to_workdir->new_file);
			std::cout << "\n";
		} else {
			std::cout << "\nNo index_to_workdir.\n";
		}
	}

	return EXIT_SUCCESS;
}


int getFileIDiffables(std::vector<IDiffable*>& diffables, po::variables_map& vm) {
	std::vector<std::string> files;
	if (vm.count("files")) {
		files = vm["files"].as< std::vector<std::string> >();
		std::cout << "\nFile Count: " << files.size() << "\n";
		std::cout << "\nFiles:\n";
		std::vector<std::string>::const_iterator iterator;
		for ( iterator = files.begin(); iterator != files.end(); ++iterator ) {
			std::cout << *iterator << "\n";
		}
	}
	if (files.size() % 2 != 0) {
		std::cerr << "ERROR: Odd number of files specified for --files.\n";
		return EXIT_FAILURE;
	}

	if (files.size() > 0) {
		for (unsigned int i = 0; i < files.size(); i += 2) {
			std::string oneFilename = files[i];
			std::string twoFilename = files[i + 1];

			diffables.push_back(new FileDiffable(oneFilename, twoFilename));

			std::cout << "\nONE:\n" << oneFilename << "\nTWO:\n" << twoFilename << "\n";
		}
	}

	return EXIT_SUCCESS;
}


int getGitIDiffables(std::vector<IDiffable*>& diffables, po::variables_map& vm) {
	std::string gitFolderPath = "/home/uberska/clarity/.git";
	Repository repo(gitFolderPath);
	std::cout << "\n>>>>> " << gitFolderPath << "\n";
	std::cout << repo.getCurrentBranchName() << "\n";

	StatusOptions statusOptions;
		std::cout << "\nABOUT TO MAKE SL\n";
	StatusList statusList = repo.getStatusList(statusOptions);
		std::cout << "\nDONE SL\n";
	for (unsigned int index = 0; index < statusList.getEntryCount(); index++) {
		std::cout << "\ngetting entry\n";
		StatusEntry entry = statusList.getEntryByIndex(index);
		Status status = entry.getStatus();

		std::cout << "\nABOUT TO MAKE ENTRY\n";

		std::cout << "\nEntry:\n" << entry.toString();

		//OId oId = entry.getHeadToIndexOldFileOId();
		//std::cout << "\nOId:\n" << oId;

		//Blob blob = repo.lookupBlob(oId);
		//std::cout << "\nBlob:\n" << blob;

		std::stringstream displayStringStream;

		if (status.isCurrent()) {
			throw std::runtime_error("When would this ever happen?");
		} else if (status.isNewInIndex()) {
			OId oldOId = entry.getHeadToIndexOldFileOId();
			OId newOId = entry.getHeadToIndexNewFileOId();

			displayStringStream << entry.getHeadToIndexNewFilePath() <<
				" (" << status.toString() << ")";

			diffables.push_back(new GitDiffable(repo, oldOId, newOId, displayStringStream.str()));
		} else if (status.isModifiedInIndex()) {
			OId oldOId = entry.getHeadToIndexOldFileOId();
			OId newOId = entry.getHeadToIndexNewFileOId();

			displayStringStream << entry.getHeadToIndexNewFilePath() <<
				" (" << status.toString() << ")";

			diffables.push_back(new GitDiffable(repo, oldOId, newOId, displayStringStream.str()));
		} else if (status.isDeletedInIndex()) {
			OId oldOId = entry.getHeadToIndexOldFileOId();
			OId newOId = entry.getHeadToIndexNewFileOId();

			displayStringStream << entry.getHeadToIndexOldFilePath() <<
				" (" << status.toString() << ")";

			diffables.push_back(new GitDiffable(repo, oldOId, newOId, displayStringStream.str()));
		} else if (status.isRenamedInIndex()) {
			OId oldOId = entry.getHeadToIndexOldFileOId();
			OId newOId = entry.getHeadToIndexNewFileOId();

			displayStringStream << entry.getHeadToIndexOldFilePath() <<
				" -> " << entry.getHeadToIndexNewFilePath() <<
				" (" << status.toString() << ")";

			diffables.push_back(new GitDiffable(repo, oldOId, newOId, displayStringStream.str()));
		} else if (status.hasTypeChangedInIndex()) {
			// TODO: needs update
			OId oldOId = entry.getHeadToIndexOldFileOId();
			OId newOId = entry.getHeadToIndexNewFileOId();
			diffables.push_back(new GitDiffable(repo, oldOId, newOId, status.toString()));
		} else if (status.isNewInWorkDir()) {
			OId oldOId = entry.getIndexToWorkDirOldFileOId();
			OId newOId = entry.getIndexToWorkDirNewFileOId();

			displayStringStream << entry.getIndexToWorkDirNewFilePath() <<
				" (" << status.toString() << ")";

			diffables.push_back(new GitDiffable(repo, oldOId, newOId, displayStringStream.str()));
		} else if (status.isModifiedInWorkDir()) {
			// TODO: needs update
			OId oldOId = entry.getIndexToWorkDirOldFileOId();
			OId newOId = entry.getIndexToWorkDirNewFileOId();
			diffables.push_back(new GitDiffable(repo, oldOId, newOId, status.toString()));
		} else if (status.isDeletedInWorkDir()) {
			// TODO: needs update
			OId oldOId = entry.getIndexToWorkDirOldFileOId();
			OId newOId = entry.getIndexToWorkDirNewFileOId();
			diffables.push_back(new GitDiffable(repo, oldOId, newOId, status.toString()));
		} else if (status.hasTypeChangedInWorkDir()) {
			// TODO: needs update
			OId oldOId = entry.getIndexToWorkDirOldFileOId();
			OId newOId = entry.getIndexToWorkDirNewFileOId();
			diffables.push_back(new GitDiffable(repo, oldOId, newOId, status.toString()));
		} else if (status.isRenamedInWorkDir()) {
			// TODO: needs update
			OId oldOId = entry.getIndexToWorkDirOldFileOId();
			OId newOId = entry.getIndexToWorkDirNewFileOId();
			diffables.push_back(new GitDiffable(repo, oldOId, newOId, status.toString()));
		} else if (status.isIgnored()) {
			throw std::runtime_error("When would this ever happen?");
		}
	}
	std::cout << "\n";

	return EXIT_SUCCESS;
}


int main(int argc, char** argv) {
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Print help message.")
		("files,f", po::value< std::vector<std::string> >()->multitoken(), "Files to diff.")
		("changes,c", "Show changes in the current git repository.")
	;

	po::positional_options_description p;
	p.add("files", -1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return EXIT_SUCCESS;
	}



	int success = EXIT_FAILURE;
	std::vector<IDiffable*> diffables;

	success = getFileIDiffables(diffables, vm);
	if (success != EXIT_SUCCESS) {
		return success;
	}

	success = getGitIDiffables(diffables, vm);
	if (success != EXIT_SUCCESS) {
		return success;
	}



	//testTextDiff();
	//testDiffMatchPatch();



	QApplication app(argc, argv);

	QWidget* window = new QWidget();

	window->setLayout(new QHBoxLayout());
	window->layout()->setContentsMargins(0, 0, 0, 0);

	QLabel* pNothingLoadedLabel = new QLabel("No diffs have been loaded.");
	pNothingLoadedLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	DiffSelectorWidget* pDiffSelectorWidget = new DiffSelectorWidget();

	std::vector<IDiffable*>::const_iterator iterator;
	for ( iterator = diffables.begin(); iterator != diffables.end(); ++iterator ) {
		pDiffSelectorWidget->addDiffable(*iterator);
	}

	QSplitter* pSplitter = new QSplitter(Qt::Horizontal);
	pSplitter->addWidget(pDiffSelectorWidget);
	pSplitter->addWidget(pNothingLoadedLabel);
	//pSplitter->setStretchFactor(0, 700);
	//pSplitter->setStretchFactor(1, 200);

	window->layout()->addWidget(pSplitter);

	int splitterSize = pSplitter->size().width();
	int splitterOffset = 150;
	QList<int> sizes;
	sizes << splitterOffset << (splitterSize - splitterOffset);
	pSplitter->setSizes(sizes);

	window->resize(900, 600);
	window->setWindowTitle("clarity");

	DiffController controller(pSplitter, pDiffSelectorWidget);

	if (pDiffSelectorWidget->count() > 0) {
		pDiffSelectorWidget->setCurrentRow(0);
	}

	window->show();

	return app.exec();
}
