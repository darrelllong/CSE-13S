import gitlab, subprocess, sys, re

TOKEN       = "H-5n-iALry3wwoEDeJ-f"
SERVER      = "https://git.ucsc.edu"
DOMAIN      = "ucsc.edu"
GROUP       = "cse0s"
SUBGROUP    = "winter2020"

def group_name(group_path):
    regex = re.compile(r'(?P<course_id>[a-zA-Z]+)(?P<course_num>[a-zA-Z0-9]+)')
    match = regex.search(group_path)
    course_id = match.group('course_id')
    course_num = match.group('course_num')
    return course_id.upper() + " " + course_num.upper()

def subgroup_name(subgroup_path):
    regex = re.compile(r'(?P<quarter>[a-zA-Z]+)(?P<year>[0-9]+)')
    match = regex.search(subgroup_path)
    quarter = match.group('quarter')
    year = match.group('year')
    return quarter.capitalize() + " " + year

def group_search(user, path):
    for group in user.groups.list():
        group_path = group.attributes['full_path']
        group_id = group.attributes['id']
        if group_path == path:
            return group
    else:
        return None

def subgroup_search(user, group_id, path):
    for subgroup in user.groups.list():
        subgroup_path = subgroup.attributes['path']
        subgroup_parent_id = subgroup.attributes['parent_id']
        if subgroup_path == path and subgroup_parent_id == group_id:
            return subgroup
    else:
        return None

def group_check(user, path):
    group = group_search(user, path)
    if not group:
        user.groups.create({
            'name': group_name(path),
            'path': path
        })
        while not group:
            group = group_search(user, path)
    return group.attributes['id']

def subgroup_check(user, group_id, path):
    subgroup = subgroup_search(user, group_id, path)
    if not subgroup:
        user.groups.create({
            'name': subgroup_name(path),
            'path': path,
            'parent_id': group_id
        })
        while not subgroup:
            subgroup = subgroup_search(user, group_id, path)
    return

def class_check(user, group, subgroup):
    subgroup_check(user, group_check(user, group), subgroup)
    return

def main():
    try:
        user = gitlab.Gitlab(SERVER, TOKEN, api_version=4)
        user.auth()
    except:
        print("Invalid token")
        sys.exit(-1)

    try:
        class_check(user, GROUP, SUBGROUP)
    except:
        print("Something went wrong checking/creating the class")
        sys.exit(-1)


if __name__ == "__main__":
    main()
