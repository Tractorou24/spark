# Style Guidelines

## Commits
A ![](docs/img/commit.png) commit title must follow the syntax `"packages: [type] Title (ID)"`

Example: `spark/mpl: [Test] Add tests for typelist::size (#10)`

- `packages`: list of “projects” modified, if more than 2 use “several”
- `[type]`: describe the nature of the commit using one of the following:
    - `[Enh]` for an enhancement
    - `[Fix]` for a bugfix (this generally goes with a referenced issue)
    - `[Refact]` for a code refactoring
    - `[Clean]` if you make the code better by only cleaning it (no refact)
    - `[Doc]` for documentation changes (in or out of code)
    - `[Test]` for adding or modifying tests
- `Title`: describe the commit in your words
- `ID`: list all concerned GitHub issues (comma-separated), if any

A commit title must start with a capital letter.  
A commit must contain a blank line between title and description.  
A commit must not contain a conflict or signed-off-by tag in the description.  
A commit must not end with a dot (before or after the issue reference)

There is no size enforcement due to the required elements in it, but please be explicit in the title.

Feel free to add any information needed to understand your change in the description. Do it like you're writing the history of your changes.  
For a fix, you can follow the Problem / Cause / Solution idiom to create a better description.

## Branches & Pull Requests

A ![](docs/img/git.png) branch must be named after the modifications made in it.  
The name must be only composed of lowercase ASCII letters and words must be separated by a dash (-).

Pull requests must have the same name as the branch. Please follow the template to ensure you don't forget anything.  
If something in it is irrelevant for you, just delete it.
