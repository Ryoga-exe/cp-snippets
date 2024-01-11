import type { CommandComment, SpecComment } from "./types.ts";

export const CONFIG_FILE = "./config.yml" as const;
export const SPEC_COMMENT: SpecComment = {
  "cpp": "/// ",
  "py": "### ",
};
export const COMMAND_COMMENT: CommandComment = {
  "cpp": ["/** ", " **/"],
  "py": [`"""* `, `* """`],
};
