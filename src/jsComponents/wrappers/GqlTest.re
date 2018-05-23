[@bs.module "../components/gqlTest"]
external reactClass : ReasonReact.reactClass = "default";

let make = children =>
  ReasonReact.wrapJsForReason(~reactClass, ~props=(), children);