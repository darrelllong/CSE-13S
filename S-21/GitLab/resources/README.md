## GitLab Resource Repo Member Adder

Script to add members to a GitLab resource repo (or really, any project).


#### Configuration

You will also need to have a JSON configuration file specific to the course that
is being setup. An example configuration file:

```json
{
  "server": "https://git.ucsc.edu",
  "domain": "ucsc.edu",
  "token": "<your GitLab token with API-access>",
  "resource_repo_id": <GitLab project ID>,
  "studentlist": "students.txt",
}
```

Here is a possible list of JSON keys:

- `server`: The GitLab server to use. The default is `git.ucsc.edu` if not
  specified.

- `domain`: The domain for the student emails. If a student's email does not
  match the set domain, their repository will not be created. The default is
  `ucsc.edu` if not specified.

- `token`: Your personal GitLab token. You can create your own token by going
  through the GitLab interface. There will be a tab for Access Tokens under your
  GitLab user settings. Your token will require the `api` and `read_api` scopes
  to be enabled in order to work properly.

- `resource_repo_id`: The project ID of the repo to add users to.

- `studentlist`: A file containing the list of students to create repositories
  for. Each line of the file should be a unique CruzID or email (the domain of
    which must match the domain you specify).

    - ```
      euchou
      ```
    - ```
      gaatorre@ucsc.edu
      ```


#### Command-line Options

This script comes with 3 command-line options:

- `-c`, `--config-file`: The path to the course's JSON configuration file. By
  default, the script will look for a file named `config.json`.
- `-l`, `--logging-level`: There are 5 options for the logging level: `debug`,
  `info`, `warning`, `error`, and `critical`. By default the logging level is
  `info`, meaning everything but debug information will be output to the
  console. To silence all output just specify the critical logging level.



#### Running

To run:

```bash
$ python3 addtoresources.py [-c configfile] [-l logginglevel]
```

When finished, the script will print out the statistics of its run: the number
of users added to the resource repo, non-existent users, errors, etc.
