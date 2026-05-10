Write-Host "============================================"
Write-Host "    2026 World Cup Flags Check Tool"
Write-Host "============================================"
Write-Host ""

$flagsDir = "flags"
$missingCount = 0

$teams = @(
    "墨西哥", "南非", "韩国", "捷克",
    "加拿大", "波黑", "卡塔尔", "瑞士",
    "巴西", "摩洛哥", "海地", "苏格兰",
    "美国", "巴拉圭", "澳大利亚", "土耳其",
    "德国", "库拉索", "科特迪瓦", "厄瓜多尔",
    "荷兰", "日本", "瑞典", "突尼斯",
    "比利时", "埃及", "伊朗", "新西兰",
    "西班牙", "佛得角", "沙特阿拉伯", "乌拉圭",
    "法国", "塞内加尔", "伊拉克", "挪威",
    "阿根廷", "阿尔及利亚", "奥地利", "约旦",
    "葡萄牙", "民主刚果", "乌兹别克斯坦", "哥伦比亚",
    "英格兰", "克罗地亚", "加纳", "巴拿马"
)

Write-Host "Required flag files:"
Write-Host "-------------------"

foreach ($team in $teams) {
    $flagPath = Join-Path $flagsDir "flag_${team}.png"
    if (Test-Path $flagPath) {
        Write-Host "[OK] flag_${team}.png"
    } else {
        Write-Host "[MISSING] flag_${team}.png"
        $missingCount++
    }
}

Write-Host ""
Write-Host "============================================"
if ($missingCount -eq 0) {
    Write-Host "All flag files are present!"
} else {
    Write-Host "Missing $missingCount flag files, please check"
}
Write-Host "============================================"

Read-Host "Press Enter to exit"