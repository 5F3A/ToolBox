# ============================================================
#  ToolBox 自动编译脚本（支持多文件夹）
#  双击 build.bat 运行
# ============================================================

$ErrorActionPreference = 'Continue'
$RootDir = $PSScriptRoot
Set-Location $RootDir

Write-Host ""
Write-Host "============================================" -ForegroundColor Cyan
Write-Host "   ToolBox 自动编译脚本" -ForegroundColor Cyan
Write-Host "   根目录: $RootDir" -ForegroundColor DarkCyan
Write-Host "============================================" -ForegroundColor Cyan
Write-Host ""

# ============ 1. 查找 Visual Studio 环境 ============
Write-Host "[1/5] 查找 Visual Studio 环境..." -ForegroundColor Yellow

$vcvars = $null

$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (Test-Path $vswhere) {
    $vsPath = & $vswhere -latest -products * `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
        -property installationPath 2>$null
    if ($vsPath) {
        $candidate = Join-Path $vsPath "VC\Auxiliary\Build\vcvars64.bat"
        if (Test-Path $candidate) { $vcvars = $candidate }
    }
}

if (-not $vcvars) {
    $candidates = @()
    $candidates += "D:\Visual Studio Build\VC\Auxiliary\Build\vcvars64.bat"
    $candidates += "D:\Visual Studio\VC\Auxiliary\Build\vcvars64.bat"
    $candidates += (Get-ChildItem "C:\Program Files\Microsoft Visual Studio\*\*\VC\Auxiliary\Build\vcvars64.bat" -ErrorAction SilentlyContinue | Select-Object -ExpandProperty FullName)
    $candidates += (Get-ChildItem "C:\Program Files (x86)\Microsoft Visual Studio\*\*\VC\Auxiliary\Build\vcvars64.bat" -ErrorAction SilentlyContinue | Select-Object -ExpandProperty FullName)
    foreach ($c in $candidates) {
        if ($c -and (Test-Path $c)) { $vcvars = $c; break }
    }
}

if (-not $vcvars) {
    Write-Host "  [X] 找不到 vcvars64.bat" -ForegroundColor Red
    Write-Host "  请确认已安装 Visual Studio 及 C++ 工作负载" -ForegroundColor Red
    Write-Host ""
    Read-Host "按回车退出"
    exit 1
}
Write-Host "  [OK] $vcvars" -ForegroundColor Green
Write-Host ""

# ============ 2. 加载 MSVC 环境变量 ============
Write-Host "[2/5] 加载 MSVC 环境变量..." -ForegroundColor Yellow
$envDump = cmd /c "call `"$vcvars`" >nul 2>&1 && set"
foreach ($line in $envDump) {
    if ($line -match '^([^=]+)=(.*)$') {
        [Environment]::SetEnvironmentVariable($Matches[1], $Matches[2], 'Process')
    }
}
Write-Host "  [OK]" -ForegroundColor Green
Write-Host ""

# ============ 3. 递归扫描源文件，选择最新版本 ============
Write-Host "[3/5] 递归扫描源文件并选择最新版本..." -ForegroundColor Yellow

# 解析规则：<Name>-<Major>.<Minor>.<Patch>[-<stage>[<num>]].cpp
#   stage: release(正式版) > snapshot(快照) > beta
function Parse-Version($fileInfo) {
    $filename = $fileInfo.Name
    if ($filename -match '^([A-Za-z][A-Za-z0-9]*)-(\d+)\.(\d+)\.(\d+)(?:-(snapshot|beta)(\d*))?\.cpp$') {
        $stage = if ($Matches[5]) { $Matches[5] } else { 'release' }
        $stageRank = switch ($stage) {
            'release'  { 3 }
            'snapshot' { 2 }
            'beta'     { 1 }
            default    { 0 }
        }
        [PSCustomObject]@{
            Name       = $Matches[1]
            Major      = [int]$Matches[2]
            Minor      = [int]$Matches[3]
            Patch      = [int]$Matches[4]
            Stage      = $stage
            StageRank  = $stageRank
            Num        = if ($Matches[6]) { [int]$Matches[6] } else { 0 }
            File       = $filename
            FullPath   = $fileInfo.FullName
            Folder     = $fileInfo.Directory.Name
        }
    }
}

# 递归扫描所有 .cpp（排除 bin/obj 等目录）
$excludeDirs = @('bin', 'obj', '.vs', '.git', 'archive')
$allCpp = Get-ChildItem -Path $RootDir -Filter "*.cpp" -Recurse -File -ErrorAction SilentlyContinue |
    Where-Object {
        $pathParts = $_.Directory.FullName.Split([IO.Path]::DirectorySeparatorChar)
        $skip = $false
        foreach ($p in $pathParts) {
            if ($excludeDirs -contains $p) { $skip = $true; break }
        }
        -not $skip
    }

Write-Host ("  找到 {0} 个 .cpp 文件" -f $allCpp.Count) -ForegroundColor DarkGray
Write-Host ""

# 按项目名分组
$projects = @{}
foreach ($f in $allCpp) {
    $v = Parse-Version $f
    if ($v) {
        if (-not $projects.ContainsKey($v.Name)) {
            $projects[$v.Name] = New-Object System.Collections.ArrayList
        }
        [void]$projects[$v.Name].Add($v)
    }
}

if ($projects.Count -eq 0) {
    Write-Host "  [X] 未找到任何可识别的源文件" -ForegroundColor Red
    Write-Host "  文件名格式应为：Name-Major.Minor.Patch[-stage].cpp" -ForegroundColor Red
    Write-Host "  例如：ToolBox-2.0.0-snapshot5.cpp" -ForegroundColor Red
    Write-Host ""
    Read-Host "按回车退出"
    exit 1
}

# ========== 方案 3 排序：先比 x.x.x，同版本内再比阶段 ==========
$latest = @{}
foreach ($name in ($projects.Keys | Sort-Object)) {
    $arr = $projects[$name]
    $sorted = $arr | Sort-Object -Property `
        @{Expression={$_.Major};     Descending=$true}, `
        @{Expression={$_.Minor};     Descending=$true}, `
        @{Expression={$_.Patch};     Descending=$true}, `
        @{Expression={$_.StageRank}; Descending=$true}, `
        @{Expression={$_.Num};       Descending=$true}
    $latest[$name] = $sorted[0]

    # 显示所有候选，选中的标 ★
    Write-Host ("  [{0}]" -f $name) -ForegroundColor Cyan
    foreach ($item in $sorted) {
        $mark = if ($item -eq $sorted[0]) { "★" } else { " " }
        $stageStr = if ($item.Stage -eq 'release') { "正式版" } else { "$($item.Stage)$($item.Num)" }
        Write-Host ("    {0} {1,-35} {2}.{3}.{4} ({5})" -f `
            $mark, $item.File, $item.Major, $item.Minor, $item.Patch, $stageStr) `
            -ForegroundColor $(if ($item -eq $sorted[0]) { "Green" } else { "DarkGray" })
    }
}
Write-Host ""

# ============ 4. 项目配置 ============
$configs = @{
    'ToolBox'    = @{ Subsystem = 'CONSOLE'; Libs = @() }
    'AlarmClock' = @{ Subsystem = 'WINDOWS'; Libs = @('user32.lib', 'gdi32.lib', 'shell32.lib') }
    'Timer'      = @{ Subsystem = 'WINDOWS'; Libs = @('user32.lib', 'gdi32.lib', 'shell32.lib') }
    'JiYuKiller' = @{ Subsystem = 'CONSOLE'; Libs = @() }
}

# ============ 5. 清理并编译 ============
Write-Host "[4/5] 清理根目录旧编译产物..." -ForegroundColor Yellow
Get-ChildItem -Path $RootDir -Filter "*.obj" -File -ErrorAction SilentlyContinue | Remove-Item -Force
Get-ChildItem -Path $RootDir -Filter "*.pdb" -File -ErrorAction SilentlyContinue | Remove-Item -Force
Write-Host "  [OK]" -ForegroundColor Green
Write-Host ""

Write-Host "[5/5] 开始编译（输出到根目录）..." -ForegroundColor Yellow
Write-Host ""

$i = 1
$total = $latest.Count
$anyFailed = $false
$exeList = @()

foreach ($name in ($latest.Keys | Sort-Object)) {
    $v = $latest[$name]
    $cfg = $configs[$name]

    if (-not $cfg) {
        Write-Host ("  [{0}/{1}] {2} → 未配置，跳过" -f $i, $total, $name) -ForegroundColor DarkYellow
        $i++
        continue
    }

    $exeName = [System.IO.Path]::GetFileNameWithoutExtension($v.File)
    $exePath = Join-Path $RootDir "$exeName.exe"
    $pdbPath = Join-Path $RootDir "$exeName.pdb"

    Write-Host ("  [{0}/{1}] {2}" -f $i, $total, $v.File) -ForegroundColor Cyan
    Write-Host ("        [{0}]" -f $v.Folder) -ForegroundColor DarkGray
    Write-Host ("        → {0}.exe" -f $exeName) -ForegroundColor DarkGray

    # 源文件为 GBK（代码页 936）
    $clArgs = @(
        '/nologo', '/EHsc', '/std:c++17', '/source-charset:gbk', '/wd4819',
        '/Zi', '/Od',
        "/Fd$pdbPath",
        $v.FullPath,
        "/Fe$exePath",
        '/link', '/DEBUG',
        "/SUBSYSTEM:$($cfg.Subsystem)"
    ) + $cfg.Libs

    & cl.exe @clArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Host "        [X] 编译失败" -ForegroundColor Red
        $anyFailed = $true
    } else {
        Write-Host "        [OK]" -ForegroundColor Green
        $exeList += "$exeName.exe"
    }
    $i++
}

Write-Host ""
if ($anyFailed) {
    Write-Host "============================================" -ForegroundColor Red
    Write-Host "   部分项目编译失败！" -ForegroundColor Red
    Write-Host "============================================" -ForegroundColor Red
} else {
    Write-Host "============================================" -ForegroundColor Green
    Write-Host "   全部编译成功！" -ForegroundColor Green
    Write-Host "============================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "  根目录生成的 exe：" -ForegroundColor White
    $exeList | Sort-Object | ForEach-Object {
        Write-Host "    $RootDir\$_" -ForegroundColor White
    }
}

Write-Host ""
Read-Host "按回车退出"