## GitLab Course Setup Script

Script to setup GitLab for a course.



#### Setup

You will first need to install the requirements for the Python script itself. To
do this, just run:

```bash
$ python3 -m pip install -r requirements.txt
```



#### Configuration

You will also need to have a JSON configuration file specific to the course that
is being setup. An example configuration file:

```json
{
  "server": "https://git.ucsc.edu",
  "domain": "ucsc.edu",
  "token": "<your GitLab token with API-access>",
  "role": "developer",
  "repo": "https://git.ucsc.edu/gaatorre/test_student.git",
  "studentlist": "students.txt",
  "course": "cse13s",
  "quarter": "winter",
  "year": "2021"
}
```

Here is a possible list of  JSON keys:

- `server`: The GitLab server to use. The default is `git.ucsc.edu` if not
  specified.

- `domain`: The domain for the student emails. If a student's email does not
  match the set domain, their repository will not be created. The default is
  `ucsc.edu` if not specified.

- `token`: Your personal GitLab token. You can create your own token by going
  through the GitLab interface. There will be a tab for Access Tokens under your
  GitLab user settings. Your token will require the `api` and `read_api` scopes
  to be enabled in order to work properly.

- `role`: The role given to the student for their repository. There are 4 roles
  possible: `developer`, `maintainer`, `master`, and `reporter`. Students
  typically just need the developer role.

- `studentlist`: A file containing the list of students to create repositories
  for. Each line of the file should be a unique CruzID or email (the domain of
    which must match the domain you specify).

    - ```
      euchou
      ```
    - ```
      gaatorre
      ```

- `studentrepolist`: A file containing shared student repositories; there are
  several students who work together using the same repository. Each line of the
  file should be the group, followed by the CruzIDs of each of the students in
  the group.

    - ``` 
      group1 student1 student2
      ```
    - ``` 
      group2 student3 student4
      ```

- `course`: The course identifier.
- `quarter`: The quarter the course is offered.
- `year`: The year the course is offered.



#### Command-line Options

This script comes with 3 command-line options:

- `-c`, `--config-file`: The path to the course's JSON configuration file. This
  is a required option.
- `-r`, `--remove-unlisted`: If enabled when creating shared student
  repositories, unlisted members who previously had access to a shared
  repository will be removed.
- `-l`, `--logging-level`: There are 5 options for the logging level: `debug`,
  `info`, `warning`, `error`, and `critical`. By default the logging level is
  `info`, meaning everything but debug information will be output to the
  console. To silence all output just specify the critical logging level.



#### Running

To run:

```bash
$ python3 repocreator.py -c <config>
```

When finished, the script will print out the statistics of its run: how many
repos were created, errors, non-existent users, etc.
