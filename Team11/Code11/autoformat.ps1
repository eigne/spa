# Define the directory to start from
$start_dir = "./src/spa/src"

# Define the Uncrustify configuration file
$uncrustify_cfg = "..\..\uncrustify.cfg"

# Recursively find all source files in the start directory
Get-ChildItem -Path $start_dir -Recurse -Include *.cpp,*.h | ForEach-Object {
    # Run Uncrustify on each file
    & uncrustify -c $uncrustify_cfg --replace --no-backup $_.FullName
}
