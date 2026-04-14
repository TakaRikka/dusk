param(
    [Parameter(Mandatory = $true)]
    [string]$InputPng,

    [Parameter(Mandatory = $true)]
    [string]$OutputIco
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

Add-Type -AssemblyName System.Drawing

$iconSizes = @(16, 24, 32, 48, 64, 128, 256)

$inputPath = (Resolve-Path $InputPng).Path
$outputPath = [System.IO.Path]::GetFullPath($OutputIco)
$outputDir = Split-Path -Parent $outputPath
if (-not [string]::IsNullOrEmpty($outputDir)) {
    [System.IO.Directory]::CreateDirectory($outputDir) | Out-Null
}

$sourceImage = [System.Drawing.Image]::FromFile($inputPath)

try {
    $entries = New-Object System.Collections.Generic.List[object]

    foreach ($size in $iconSizes) {
        $bitmap = New-Object System.Drawing.Bitmap $size, $size
        try {
            $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
            try {
                $graphics.Clear([System.Drawing.Color]::Transparent)
                $graphics.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
                $graphics.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
                $graphics.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
                $graphics.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
                $graphics.DrawImage($sourceImage, 0, 0, $size, $size)
            } finally {
                $graphics.Dispose()
            }

            $memory = New-Object System.IO.MemoryStream
            try {
                $bitmap.Save($memory, [System.Drawing.Imaging.ImageFormat]::Png)
                $entries.Add([pscustomobject]@{
                    Size = $size
                    Bytes = $memory.ToArray()
                }) | Out-Null
            } finally {
                $memory.Dispose()
            }
        } finally {
            $bitmap.Dispose()
        }
    }

    $fileStream = [System.IO.File]::Open($outputPath, [System.IO.FileMode]::Create, [System.IO.FileAccess]::Write)
    try {
        $writer = New-Object System.IO.BinaryWriter $fileStream
        try {
            $writer.Write([UInt16]0)
            $writer.Write([UInt16]1)
            $writer.Write([UInt16]$entries.Count)

            $dataOffset = 6 + (16 * $entries.Count)
            foreach ($entry in $entries) {
                $dimension = if ($entry.Size -ge 256) { 0 } else { [byte]$entry.Size }
                $writer.Write([byte]$dimension)
                $writer.Write([byte]$dimension)
                $writer.Write([byte]0)
                $writer.Write([byte]0)
                $writer.Write([UInt16]1)
                $writer.Write([UInt16]32)
                $writer.Write([UInt32]$entry.Bytes.Length)
                $writer.Write([UInt32]$dataOffset)
                $dataOffset += $entry.Bytes.Length
            }

            foreach ($entry in $entries) {
                $writer.Write($entry.Bytes)
            }
        } finally {
            $writer.Dispose()
        }
    } finally {
        $fileStream.Dispose()
    }
} finally {
    $sourceImage.Dispose()
}
