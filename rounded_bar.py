from PIL import Image, ImageDraw

width = 1200
height = 200

image = Image.new("RGBA", (width, height), (0, 0, 0, 0))
fillImage = Image.new("RGBA", (width, height), (0, 0, 0, 0))

draw = ImageDraw.Draw(image)
fillDraw = ImageDraw.Draw(fillImage)

outlineWidth = 16

barBounds = [0, 0, width, height]

cornerRadius = (height - outlineWidth) / 2
color = "#EE2222"

fillBarBounds = [outlineWidth, outlineWidth, width - outlineWidth, height - outlineWidth]
fillRadius = max(0, cornerRadius - outlineWidth / 2)

draw.rounded_rectangle(barBounds, radius = cornerRadius, outline = color, width = outlineWidth)
fillDraw.rounded_rectangle(fillBarBounds, radius = fillRadius, fill = "#FF00FF")

image.save("bar.png", "PNG")
fillImage.save("bar-fill.png", "PNG")
