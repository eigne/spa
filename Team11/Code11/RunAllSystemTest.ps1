param($AutoTesterPath)

if (-not $AutoTesterPath) {
    Write-Host "Error: Please provide the AutoTester path as an argument."
    exit 1
}

# Get all test folder names in the ./tests folder
$testFolders = Get-ChildItem -Path ./../Tests11 -Directory | ForEach-Object { $_.Name }
# Keep count of number of tests that failed
$totalFailed = 0

$analysisXslPath = Join-Path -Path ./tests -ChildPath "analysis.xsl"

foreach ($testFolder in $testFolders) {
    # Powershell script way of running the autotester
    $output = & $AutoTesterPath "./../Tests11/$testFolder/${testFolder}_source.txt" "./../Tests11/$testFolder/${testFolder}_queries.txt" "./tests/${testFolder}_output.xml" 2>&1

    $errCode = $LASTEXITCODE
    $missing = $output | Select-String "Missing:"
    $additional = $output | Select-String "Additional:"

    if ($missing -and $additional) {
        Write-Host "${testFolder}_queries.txt failed"
        $totalFailed++
    }

    if ($errCode -ne 0) {
        Write-Host "${testFolder}_source.txt either failed parsing or timed out"
        $totalFailed++
    }
}

Write-Host "Total failed tests: $totalFailed"
