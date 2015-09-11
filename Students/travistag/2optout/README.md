# cURL / JSON

Making this project will create two executables:

###cURLToJSON
This executable is run with no arguments and will curl data from a college football rankings page, and pack this information into a JSON object, rankings.json
It will also generate a file called rankingsrawdata.txt, which contains the raw curl data.

###JSONtoMarkdown
This executable will take in the rankings.json object, and output a Markdown file, rankings.md, which can be viewed in this repository on GitHub.
