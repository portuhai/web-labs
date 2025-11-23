function Select({ label, options }) {
  return (
    <div className="select-group">
      <label>{label}</label>
      <select>
        {options.map((o) => (
          <option key={o}>{o}</option>
        ))}
      </select>
    </div>
  );
}

export default Select;
