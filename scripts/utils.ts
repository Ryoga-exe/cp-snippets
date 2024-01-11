import { extname } from "std/path/mod.ts";
import type { Config, Snippet } from "./types.ts";
import { COMMAND_COMMENT, SPEC_COMMENT } from "./consts.ts";

export function generateSnippet(
  path: string,
  snippet: Snippet,
  config: Config,
) {
  const ext = extname(path);
  const languageScope = config.scope[ext];
  const content = COMMAND_COMMENT[languageScope].reduce(
    (lines, comment) => lines.replaceAll(comment, ""),
    Deno.readTextFileSync(path),
  );
  const start = SPEC_COMMENT[languageScope] + "start";
  const end = SPEC_COMMENT[languageScope] + "end";
  const spec = SPEC_COMMENT[languageScope] + "@";
  let lines: string[] = [];
  let current: string;
  let prefix = "";
  let description = "";
  let isFileTemplate = false;
  content.split("\n").forEach((line) => {
    const trimmed = line.trim();
    if (trimmed.startsWith(start)) {
      lines = [];
      current = trimmed.slice(start.length).trim();
    } else if (trimmed.startsWith(end)) {
      const name = current ? `${path}-${current}` : path;
      snippet[name] = {
        prefix,
        description,
        body: lines,
        isFileTemplate,
        scope: languageScope,
      };
      current = "";
      prefix = "";
      description = "";
      isFileTemplate = false;
      lines = [];
    } else if (trimmed.startsWith(spec)) {
      const [key, value] = trimmed.slice(spec.length).split(/(?<=^[^:]+):/).map(
        (entry) => entry.trim(),
      );
      if (key === "prefix") {
        prefix = value;
      } else if (key === "description") {
        description = value.replaceAll("\\n", "\n");
      } else if (key === "filetemplate") {
        isFileTemplate = value.toLowerCase() === "true";
      }
    } else {
      lines.push(line);
    }
  });
  if (isFileTemplate) {
    snippet[path] = {
      prefix,
      description,
      body: lines,
      isFileTemplate,
      scope: languageScope,
    };
  }
}
