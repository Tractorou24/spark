"""Script used to lint commits in a repository or manually specified commits."""

import argparse
import re
import subprocess
import sys
from typing import List


def get_commits(folder: str, reference: str) -> List[str]:
    """Get commits from the specified git reference to HEAD.

    Arguments:
    ---------
        folder (str): The path to the git repository
        reference (str): The git reference to start checking from

    Returns:
    -------
        commits (List[str]): The commits from the specified git reference to HEAD
    """
    command = f"git rev-list {reference}..HEAD"
    commits_ids = subprocess.check_output([command], shell=True, cwd=folder).decode("utf-8").splitlines()

    commits = []
    empty_lines_number_to_remove = (1, 4)
    for commit_id in commits_ids:
        command = f"git show -s --pretty=full {commit_id} | sed {empty_lines_number_to_remove[0]},{empty_lines_number_to_remove[1]}d"
        commit_lines = subprocess.check_output([command], shell=True, cwd=folder).decode("utf-8").splitlines()
        commit_lines = list(map(str.strip, commit_lines))
        commits.append("\n".join(commit_lines))
    return commits


def check_commit(commit: str) -> List[str]:
    """Check if commit message is valid.

    Arguments:
    ---------
        commit (str): The commit message to check

    Returns:
    -------
        errors (List[str]): The errors found in the commit message
    """
    errors = []
    if not re.search(r"^[a-zA-Z-0-9, \/._]+: ", commit):
        errors.append("Commit message is missing package name in commit title (if this is a fix up of a previous commit, it should be squashed)")
    if not re.search(r"\[(Fix|Enh|Test|Refact|Doc|Clean|Backport)]", commit):
        errors.append("Commit message does not contains type or type is invalid (valid types are: Fix, Enh, Test, Refact, Doc, Clean, Backport)")
    if not re.search(r"^.+(\r?\n(\r?\n.*)*)?$", commit):
        errors.append("Empty line between commit title and body is missing")
    if not re.fullmatch(r"^.+(?<![.\n])( \(#[0-9]+\))(\r?\n.*)*", commit) and not re.search(r"^.+[^.\n]+(\r?\n.*)*$", commit):
        errors.append("Commit message ends with a period")
    if not commit[commit.index("] ") + 2].isupper():
        errors.append("Commit title is not capitalized")
    if commit.find("Merge branch") != -1:
        errors.append("Commit is a git merge commit, use the rebase command instead")
    if commit.find("Conflicts:") != -1:
        errors.append("Commit message contains a git conflict marker")
    if commit.find("Signed-off-by:") != -1:
        errors.append("Commit body contains a Signed-off-by tag")
    return errors


def show_errors(errors: List[str]) -> None:
    """Show errors in a nice way.

    Arguments:
    ---------
        errors (List[str]): The errors to show
    """
    if len(errors) == 0:
        return

    print("\033[91mCommit Linting Errors:\033[0m")
    for error in errors:
        print(error)


def handle_commits(commits: List[str]) -> bool:
    """Test every commit, print errors and return if there are any errors.

    Arguments:
    ---------
        commits (List[str]): The commits to test

    Returns:
    -------
        has_errors (bool): If there are any errors
    """
    errors = []
    for commit in commits:
        if commit == "":
            errors.append("\u001b[33m    - Empty commit is invalid!\033[0m")
            continue
        error_list = check_commit(commit)
        if len(error_list) > 0:
            errors.append(f"    - Commit \u001b[33m{commit.splitlines()[0]}\033[0m has following error{'s' if len(error_list) > 1 else ''}:")
            for error in error_list:
                errors.append(f"        {error}")

    show_errors(errors)
    return len(errors) > 0


def main(args: List[str]) -> int:
    """Handle getting the commits and checking them."""
    parser = argparse.ArgumentParser(description="Lints all commits specified whth SPARK rules.")
    subparsers = parser.add_subparsers(dest="command", required=True)

    repo_parser = subparsers.add_parser("repository")
    repo_parser.add_argument("repository_path")
    repo_parser.add_argument("sha_id")

    manual_parser = subparsers.add_parser("manual")
    manual_parser.add_argument("commits", type=str, nargs="+")

    arguments = parser.parse_args(args)

    if arguments.command == "manual":
        code = handle_commits(arguments.commits)
        if code == 0:
            print(f"\u001b[32m{'All commits are' if len(args[1:]) > 1 else 'Commit is'} valid!\033[0m")
        return code

    commit_list = get_commits(arguments.repository_path, arguments.sha_id)
    code = handle_commits(commit_list)
    if code == 0:
        print(f"\u001b[32m{'All commits are' if len(commit_list) > 1 else 'Commit is'} valid!\033[0m")
    return code


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
