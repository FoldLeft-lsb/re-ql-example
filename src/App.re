[%bs.raw {|require('./App.css')|}];

[@bs.module] external logo : string = "./logo.svg";

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: _self =>
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <h2> (str(message)) </h2>
      </div>
      <hr />
      <GqlTest />
      <ReqlTest />
    </div>,
};