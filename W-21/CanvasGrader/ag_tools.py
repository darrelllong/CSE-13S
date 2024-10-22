import subprocess as sp

#some config stuff:
DEFAULT_OUTPUT_LINES = 12 #number of lines of student output to include
DEFAULT_TIMEOUT = 15

#terminal codes:
black = "\u001b[30m"
red = "\u001b[31m"
orange = "\u001b[91m" #technically it's 'light red' but whatever
green = "\u001b[32m"
yellow = "\u001b[33m"
blue = "\u001b[34m"
magenta = "\u001b[35m"
cyan = "\u001b[36m"
white = "\u001b[37m"
reset = "\u001b[0m"

#verbosities:
OVERVIEW = 0
GRADING = 1
DEBUG = 2
verbosities = {"OVERVIEW":OVERVIEW, "GRADING":GRADING, "DEBUG":DEBUG}

#indent levels:
TALLY = 3
RESULT = 2
TEST = 1
SECTION = 0

#scoring methods:
NONE = None
LINEAR = 1
STRICT = 2
GENEROUS = 3

class Tester:
  """The Tester class is meant to provide a way to conduct tests such that
  the results can be printed with consistent formatting and tabulation.
  See the design doc for more detail.
  """
  def __init__(self, cruzid, asgn, asgn_dir, verbosity="GRADING"):
    self.cruzid = cruzid
    self.asgn = asgn
    self.asgn_dir = asgn_dir

    self.printout = ""
    self.sections = []
    self.set_verbosity(verbosity)
    self.indent_level = SECTION


  def set_verbosity(self, verbosity):
    f"""sets the verbosity of the Results object.
    Must be one of {[v for v in verbosities.keys()]}.
    """
    if verbosity in verbosities.keys():
      self.verbosity = verbosities[verbosity]
    else:
      raise Exception(f"Verbosity must be one of {[k for k in verbosities.keys()]}")


  def start_section(self, title, description=""):
    """used to begin a new section of testing.  If "points" is set to none, no tabulations will be performed"""
    self.current_section_tests = []
    self.print(OVERVIEW, f"-------------{title}-------------", indent_level=SECTION, color=blue)
    self.print(OVERVIEW, description, indent_level=SECTION)
    self.current_section = {"title":title, "errors":False}
    self.indent_level = TEST

  def start_test(self, title, description=None, weight=1):
    """begin a new test"""
    self.print(OVERVIEW, title, indent_level=TEST)
    self.print(OVERVIEW, description)
    self.current_test = {"title":title, "weight":weight}
    self.indent_level = RESULT

  def end_test(self, conditional, fail_explain = "", range=None):
    """end current test"""
    if not range:
      if conditional:
        self.print(OVERVIEW, f"- {green}passed{white}")
        self.current_test["score"] = 1
      else:
        if fail_explain and not fail_explain.isspace():
            self.print(OVERVIEW, f"- {red}FAILED{white} - {fail_explain}")
        else:
            self.print(OVERVIEW, f"- {red}FAILED{white}")
        self.current_section["errors"] = True
        self.current_test["score"] = 0
    else:
      if conditional == range[0]:
        self.print(OVERVIEW, f"- {green}passed{white}")
      elif conditional == range[1]:
        self.print(OVERVIEW, f"- {red}FAILED{white} - {fail_explain}")
        self.current_section["errors"] = True
      else:
        self.print(OVERVIEW, f"- {orange}partial success{white} {abs(conditional-range[1])}/{abs(range[0]-range[1])} - {fail_explain}")
        self.current_section["errors"] = True
      try:
          range_score = float(conditional - range[1])/(range[0]-range[1])
      except:
          range_score = 0 # There's a division by zero error here.
      if range_score > 1 or range_score < 0:
        self.print(GRADING, "got value {conditional} which is not in range {range}, clamping")
      self.current_test["score"] = min(1,max(range_score, 0))
    self.current_section_tests.append(self.current_test)
    self.current_test = None
    self.indent_level=TEST


  def end_section(self,scoring_method = None, points = None):
    """used to end a section.  At this point the scoring method is specified
    """
    self.indent_level = TALLY
    self.print(OVERVIEW, "---------------------------------", indent_level = SECTION)

    max_points=points
    self.current_section["max_score"] = max_points
    scoring_expr = {None: "no scoring method specified",
                    LINEAR:  f"section scored as ({max_points} * normalized_score)",
                    STRICT:  f"section scored as ({max_points} * normalized_score^2)",
                    GENEROUS:f"section scored as ({max_points} * sqrt(normalized_score)"
                            }[scoring_method]
    self.print(OVERVIEW, f"""{self.current_section["title"]} complete, {scoring_expr}""")

    if scoring_method != None:
      if not self.current_section["max_score"]:
        raise(Exception(f"""section {self.current_section["title"]} has no points, but a scoring method was specified.  Change one of these things please!"""))
      #sum tests:
      raw_score = sum([test["score"]*test["weight"] for test in self.current_section_tests])
      test_strs = " + ".join([f"""\n > (weight {t["weight"]}) * (score {t["score"]}) for {t["title"]}""" for t in self.current_section_tests])
      self.print(DEBUG, f"""raw_section_score = {raw_score} = \n{test_strs}""")

      #normalize:
      weight_sum =  sum([test["weight"] for test in self.current_section_tests])
      norm_score = raw_score/weight_sum
      self.print(DEBUG, f"""normalized score = raw_section_score / weight_sum = {raw_score}/{weight_sum} = {norm_score}""")

      #apply scoring method:
      if scoring_method==LINEAR:
        real_score = max_points * norm_score
      if scoring_method==STRICT:
        real_score = max_points * (norm_score)**2
      if scoring_method==GENEROUS:
        real_score = max_points * (norm_score)**0.5

      self.current_section["score"] = real_score
      self.print(OVERVIEW, f"final section score = {real_score}")
    else:
      self.current_section["score"] = None

    self.sections.append(self.current_section)
    self.current_section = None
    self.indent_level=SECTION

  def generate_summary(self):
    self.print(OVERVIEW, f"{blue} *********************SUMMARY*****************{white}")
    tot_score = 0
    tot_max_score = 0
    for sec in self.sections:
      err_str = f"{red}! ERRORS{white}" if sec["errors"] else f"{green}no errors{white}"
      if sec["max_score"]:
        score_str = f"""{sec["score"]:.2f} / {sec["max_score"]:.2f}"""
        tot_score += sec["score"]
        tot_max_score += sec["max_score"]
      else:
        score_str = "   unscored"

      self.print(OVERVIEW, f"""     {score_str:16}|  {sec["title"]:30} | {err_str}""")
    self.print(OVERVIEW,   f"""Total:   {tot_score:.2f} / {tot_max_score:.2f}""")

  def run_command(self, cmd, timeout=DEFAULT_TIMEOUT, return_all=False,
                  output_lines = DEFAULT_OUTPUT_LINES):
    f"""a wrapper for subprocess.run() that captures io for presentation to grader.
    cmd:  command to run
    timeout:  timeout in seconds (default {DEFAULT_TIMEOUT})
    return_all:  If false (default), run_command returns all output from cmd.  Otherwise, it will return a tuple (output, stderr, exit_code) where exit_code is the exit code of cmd
    """
    self.print(GRADING, cmd, color=yellow)
    try:
      proc = sp.run(cmd, capture_output=True, shell=True, timeout=timeout)
    except Exception as e:
      self.print(GRADING, f">> error (from python): {e}", max_lines = output_lines, color=magenta)
      return "" #not much else to do here, huh?
    err = proc.stderr.decode("utf-8")
    if err:
      self.print(GRADING, f">> error (from stderr): $> {err}", max_lines=output_lines, color=magenta)

    output = proc.stdout.decode("utf-8")
    self.print(DEBUG, f"exit code:{proc.returncode}")
    self.print(GRADING, output, color=cyan, max_lines=output_lines)

    if return_all:
      return (output, err, proc.returncode)
    else:
      return output


  def print(self, verbosity, string,
            indent_level = -1, max_lines = -1, color = None):
    """print(verbosity, string) prints whatever string is passed, as well as adding that string to the printout log, iff the verbosity is high enough.   It also applies a certain amount of formatting:  It applies an adjustable indentation level, and optionally applies color and truncates the printed output.  max_lines limits the number of lines printed, if -1 then all lines are printed."""

    #first check verbosity and presence of string
    if self.verbosity < verbosity:
      return
    if not string:
      return

    if indent_level < 0:
      indent_level = self.indent_level

    #now we do a bit of formatting
    string_lines = [l for l in string.split("\n") if l.strip()]
    if max_lines != -1:
      if len(string_lines) > max_lines:
        extra_lines = len(string_lines) - max_lines
        end_lines = int(max_lines/2)
        front_lines = max_lines - end_lines
        string_lines = string_lines[:front_lines] + [f"...({extra_lines} lines of output suppressed)..."] + string_lines[-(end_lines):][:end_lines]
    s = ["   "*indent_level + l for l in string_lines]
    s = "\n".join(s)
    if color:
      s = color + s + white
    self.printout += s
    print(s, flush=True)


"""
fuzzy_s() is a fuzzy string matcher, it calculates levenshtein distance between token1 and token2.
Useful for checking if student output is CLOSE to a target, while recovering from format errors.
So for example, if the student was SUPPOSED to output
"Hello World!\n"
but actually output
"hello world"
Then fuzzy_s() would yield a value of 3.  But if the student actually output nothing, fuzzy_s would yield 13.  So this provides a way to give partial credit for improperly formatted output.

fuzzy_s is O(n^2), which means its only useful for short strings (<10k).  See fuzzy_l for
an alternative more suitable for some longer comparisons.

inputs:
   token1, token2:  Two strings to compare
outputs:
  edit_distance:  The levenshtein distance between token1 and token2
"""
def fuzzy_s(token1, token2):
    #error if its too long:
    if( max(len(token1), len(token2)) > 8000):
      print("    Input token(s) too long for short fuzzy match!")
      return(max(len(token1),len(token2)))
    #print("tokens:",[token1, token2])

    #token1 is the rows, token2 is the cols
    prev_row = [i for i in range(len(token2)+1)];
    curr_row = [1 for i in range(len(token2)+1)];

    for row in range(1, len(token1) + 1):
        #print(prev_row)
        for col in range(1, len(token2) + 1):
            if (token1[row-1] == token2[col-1]):
                ind = 0
            else:
                ind = 1

            curr_row[col] = min(
                curr_row[col-1] + 1,
                prev_row[col] + 1,
                prev_row[col-1] + ind)
        prev_row = curr_row
        curr_row = [row + 1 for i in range(len(token2)+1)]

    edit_distance = prev_row[-1]
    return prev_row[-1]


"""
fuzzy_l() is a second fuzzy string matcher, useful for longer strings (>100 characters)

it works by finding several "marker" sequences throughout the target string,
and searching the dest string for those sequences.   This was designed for use in
the LZW lab, to operate on strings that are sequences of 0 and 1.  In that lab, students
often produce files that contain some compoennts of the target file, such as the first
or last 4kb.  This is a way to give partial credit in these cases.  It may be useful in other situations, though, and is included here as a compliment to fuzzy_s.

unlike fuzzy_s, it's asymmetrical:    target should be the correct version.
Also,  unlike fuzzy_s, 0 is the worst possible score!

If seq_length is not provided, then fuzzy_l attempts to determine a suitable seq_length
that with an approximate 1% false positive rate, assuming that the target string is
more or less random, and consists of only 2 types of characters.  This assumption makes sense
when the target is a compressed file, but you'll probably want a different approach
for other types of strings.

input:
  target:         correct string
  dest:           student string
  [n_seq]:        number of substrings to search for
  [seq_len]:   length in characters of each substring
returns:
  (matches, n_seq):
  matches:        number of target substrings that were present in dest
  n_seq:          total number of target substrings (ie, the "max score")
"""
def fuzzy_l(target, dest, n_seq = 10, seq_len = None):
   import re
   from math import log

   if len(dest) < 100:
     print(f"      dest string is {len(dest)} bits, too short for fuzzy matching!")
     return(0,0)

   matches = 0
   fp_desired = 0.01

   try:
     if not seq_len:
       #approx odds of false positive are 1-(1 - 2^-sequence_len)^(dest_len)
       #or, solving for sequence len:
       # sequence_len = -log2 ( 1 - (1-fp_desired)^(1/dest_len))
       seq_len = -int( log(1 - (1-fp_desired)**(1.0/len(dest))) /log(2))
       #print(f"       len(target)={len(target)} bits, len(dest)={len(dest)} bits)
       #print(f"       finding {n_seq} seqs of {seq_len} bits each")

     start_range = len(target)-seq_len-1
     start_indexes = [int(i*start_range/(n_seq-1)) for i in range(n_seq-1)] + [start_range]
     #print(f"   start indexes:  {start_indexes}")

     for i in start_indexes:
        target_substring = target[i:(i+seq_len)]
        if re.search(target_substring, dest):
           matches += 1

   except Exception as e:
     print(f"      Error in fuzzy matching:  {e} (target_len={len(target)}, dest_len={len(dest)})")
     return(matches, n_seq)

   return (matches, n_seq)
