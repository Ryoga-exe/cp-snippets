import { parse } from "std/yaml/mod.ts";
import { walkSync } from "std/fs/walk.ts";
import { globToRegExp, join } from "std/path/mod.ts";
import type { Config, Snippet } from "./types.ts";
import { CONFIG_FILE } from "./consts.ts";
import { generateSnippet } from "./utils.ts";

const config = parse(Deno.readTextFileSync(CONFIG_FILE)) as Config;
const ignoreFiles = config.ignore.map((ignoreFile) => globToRegExp(ignoreFile));
const files = walkSync(config.root, { skip: ignoreFiles });
const result: Snippet = {};
for (const file of files) {
  if (file.isFile) {
    console.log("generate", file.path);
    generateSnippet(file.path, result, config);
  }
}
Deno.mkdirSync(config.output, { recursive: true });
Deno.writeTextFileSync(join(config.output, "vscode.code-snippets"), JSON.stringify(result));
