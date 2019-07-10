{
	"targets": [
	{
		"target_name": "mcp2221native",
		"sources": [ "mcp2221native.cc", "mcp2221.c", "hid.c", "libmcp2221.c", "utility.cc"],
		"libraries": [
			"-ludev"
		]
	}
	]
}
